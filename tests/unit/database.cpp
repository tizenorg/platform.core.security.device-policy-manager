/*
 *  Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License
 */

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

#include <iostream>
#include <string>
#include <stdexcept>

#include "exception.h"

#include "db/connection.h"
#include "db/statement.h"
#include "db/column.h"

#include "testbench/testbench.h"

TESTCASE(DatabaseTest)
{
	std::string query = "CREATE TABLE IF NOT EXISTS CLIENT("     \
						"ID INTEGER PRIMARY KEY AUTOINCREMENT,"  \
						"PKG TEXT,"                              \
						"KEY TEXT,"                              \
						"IS_USED INTEGER,"                       \
						"USER INTEGER)";

	try {
		database::Connection db("/tmp/test.db", database::Connection::ReadWrite | database::Connection::Create);
		db.exec(query);
	} catch (runtime::Exception& e) {
		TEST_FAIL(e.what());
	}
}

TESTCASE(InvalidStatementTest)
{
	try {
		database::Connection db("/tmp/test.db", database::Connection::ReadWrite | database::Connection::Create);
		database::Statement stmt(db, "INVALID STATEMENT");
	} catch (runtime::Exception& e) {
	}
}

TESTCASE(ColumnBindTestWithIndex1)
{
	std::string query = "INSERT INTO CLIENT VALUES (NULL, ?, ?, ?, ?)";

	try {
		const char *str = "PACKAGE";
		void *blob = (void *)str;
		double user = 5001;
		sqlite3_int64 used = 1;
		std::string key = "test key";

		database::Connection db("/tmp/test.db", database::Connection::ReadWrite | database::Connection::Create);
		database::Statement stmt(db, query);
		stmt.bind(1, blob, 8);
		stmt.bind(2, key);
		stmt.bind(3, used);
		stmt.bind(4, user);
		stmt.exec();
		database::Statement select(db, "SELECT * FROM CLIENT");

		TEST_EXPECT(5, select.getColumnCount());
		stmt.clearBindings();
		stmt.reset();
	} catch (runtime::Exception& e) {
		TEST_FAIL(e.what());
	}
}

TESTCASE(ColumnBindTestWithIndex2)
{
	std::string query = "INSERT INTO CLIENT VALUES (NULL, ?, ?, ?, ?)";

	try {
		database::Connection db("/tmp/test.db", database::Connection::ReadWrite | database::Connection::Create);
		database::Statement stmt(db, query);
		stmt.bind(1, "TEST PACKAGE");
		stmt.bind(2, "TEST KEY");
		stmt.bind(3, false);
		stmt.bind(4, 5001);
		stmt.exec();
	} catch (runtime::Exception& e) {
		TEST_FAIL(e.what());
	}
}

TESTCASE(ColumnBindTestWithName1)
{
	std::string query = "INSERT INTO CLIENT VALUES (NULL, :PKG, :KEY, :IS_USED, :USER)";

	try {
		database::Connection db("/tmp/test.db", database::Connection::ReadWrite | database::Connection::Create);
		database::Statement stmt(db, query);
		stmt.bind(":PKG", "TEST PACKAGE");
		stmt.bind(":KEY", "TEST KEY");
		stmt.bind(":IS_USED", true);
		stmt.bind(":USER", 5001);
		stmt.exec();
	} catch (runtime::Exception& e) {
		TEST_FAIL(e.what());
	}
}

TESTCASE(ColumnBindTestWithName2)
{
	std::string query = "INSERT INTO CLIENT VALUES (NULL, :PKG, :KEY, :IS_USED, :USER)";

	try {
		const char *str = "PACKAGE";
		void *blob = (void *)str;
		double user = 5001;
		sqlite3_int64 used = 1;
		std::string key = "test key";

		database::Connection db("/tmp/test.db", database::Connection::ReadWrite | database::Connection::Create);
		database::Statement stmt(db, query);
		stmt.bind(":PKG", blob, 8);
		stmt.bind(":KEY", key);
		stmt.bind(":IS_USED", used);
		stmt.bind(":USER", user);
		stmt.exec();
	} catch (runtime::Exception& e) {
		TEST_FAIL(e.what());
	}
}

TESTCASE(ColumnTest)
{
	try {
		database::Connection db("/tmp/test.db", database::Connection::ReadWrite | database::Connection::Create);
		database::Statement select(db, "SELECT * FROM CLIENT");
		while (select.step()) {
			for (int  i = 0; i < select.getColumnCount(); i++) {
				TEST_EXPECT(false, select.isNullColumn(i));
				std::cout << "C: " << select.getColumnName(i);
			}
			std::cout << std::endl;

			database::Column id = select.getColumn(0);
			database::Column pkg = select.getColumn(1);
			database::Column key = select.getColumn(2);
			database::Column used = select.getColumn(3);

			std::cout << "Column: " << id.getName()
					  << ", Int: " << id.getInt()
					  << ", Int64: " << id.getInt64()
					  << ", Double: " << id.getDouble()
					  << ", Type: " << id.getType()
					  << ", Bytes: " << id.getBytes()
					  << ", Double: " << id.getDouble()
					  << std::endl;
			std::cout << "Column: " << pkg.getName()
					  << ", Text: " << pkg.getText()
					  << ", Type: " << pkg.getType()
					  << ", Bytes: " << pkg.getBytes()
					  << ", Blob: " << (const char*)pkg.getBlob()
					  << std::endl;
			std::cout << "Column: " << key.getName()
					  << ", Text: " << key.getText()
					  << ", Type: " << key.getType()
					  << ", Bytes: " << key.getBytes()
					  << ", Blob: " << (const char*)key.getBlob()
					  << std::endl;
			std::cout << "Column: " << used.getName()
					  << ", Int: " << used.getInt()
					  << ", Int64: " << used.getInt64()
					  << ", Double: " << used.getDouble()
					  << ", Type: " << used.getType()
					  << ", Bytes: " << used.getBytes()
					  << std::endl;
		}
	} catch (runtime::Exception& e) {
		TEST_FAIL(e.what());
	}
}

TESTCASE(InvalidDB)
{
	try {
		database::Connection db("/tmp/invalid.db", database::Connection::ReadWrite);
	} catch (runtime::Exception& e) {
	}
}
