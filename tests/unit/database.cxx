// Copyright (c) 2015 Samsung Electronics Co., Ltd.
//
// Licensed under the Apache License, Version 2.0 (the License);
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>

#include <iostream>
#include <string>
#include <stdexcept>

#include "exception.hxx"

#include "db/connection.hxx"
#include "db/statement.hxx"
#include "db/column.hxx"

#include "testbench/testbench.hxx"

TESTCASE(DatabaseTest)
{
    std::string query = "CREATE TABLE IF NOT EXISTS CLIENT("     \
                        "ID INTEGER PRIMARY KEY AUTOINCREMENT,"  \
                        "PKG TEXT,"                              \
                        "KEY TEXT,"                              \
                        "IS_USED INTEGER)";

    try {
        Database::Connection db("/tmp/test.db", Database::Connection::ReadWrite | Database::Connection::Create);
        db.exec(query);
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }
}

TESTCASE(StatementTest)
{
    std::string query = "INSERT INTO CLIENT VALUES (NULL, ?, ?, ?)";

    try {
        Database::Connection db("/tmp/test.db", Database::Connection::ReadWrite | Database::Connection::Create);
        Database::Statement stmt(db, query);
        stmt.bind(1, "test package");
        stmt.bind(2, "test key");
        stmt.bind(3, false);
        stmt.exec();
        Database::Statement select(db, "SELECT * FROM CLIENT");

        TEST_EXPECT(4, select.getColumnCount());

    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }
}

TESTCASE(ColumnTest)
{
    std::string query = "INSERT INTO CLIENT VALUES (NULL, ?, ?, ?)";

    try {
        Database::Connection db("/tmp/test.db", Database::Connection::ReadWrite | Database::Connection::Create);
        Database::Statement stmt(db, query);
        stmt.bind(1, "test package");
        stmt.bind(2, "test key");
        stmt.bind(3, false);
        stmt.exec();
        Database::Statement select(db, "SELECT * FROM CLIENT");
        select.step();
        Database::Column id = select.getColumn(0);
        Database::Column pkg = select.getColumn(1);
        Database::Column key = select.getColumn(2);
        Database::Column used = select.getColumn(3);

        std::cout << "Id Column Name: " << id.getName() << std::endl;
        std::cout << "Pkg Column Name: " << pkg.getName() << std::endl;
        std::cout << "Key Column Name: " << key.getName() << std::endl;
        std::cout << "Used Column Name: " << used.getName() << std::endl;

        std::cout << "Id Column Value: " << id.getInt() << std::endl;
        std::cout << "Pkg Column Value: " << pkg.getText() << std::endl;
        std::cout << "Key Column Value: " << key.getText() << std::endl;
        std::cout << "Used Column Value: " << used.getInt() << std::endl;
    } catch (Runtime::Exception& e) {
        TEST_FAIL(e.what());
    }
}

