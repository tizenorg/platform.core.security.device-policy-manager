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

#ifndef __TESTBENCH__
#define __TESTBENCH__

#include <cstring>

#include <string>
#include <vector>
#include <memory>
#include <sstream>

namespace Test {

struct Source {
    Source(const std::string& file, long line, const std::string& msg);

    std::string fileName;
    long lineNumber;
    std::string	message;
};

class TestResult {
public:
    TestResult();
    virtual ~TestResult();
    virtual void testsStarted();
    virtual void addFailure(const std::string& name, const Source& source);
    virtual void testsEnded();

private:
    int	failureCount;
};

class TestSuite {
public:
    TestSuite(const std::string& name);
    virtual ~TestSuite();

    TestSuite(const TestSuite&) = delete;
    TestSuite& operator=(const TestSuite&) = delete;

    void run();

    const std::string& name() const
    {
        return testName;
    }

protected:
    typedef void (TestSuite::*TestFunction)();

    struct TestCase {
        TestCase(TestFunction func, const std::string& name)
            : function(func), testName(name)
        {
        }

        TestFunction function;
        std::string testName;
    };

    virtual void setup();
    virtual void teardown();

#define addTest(func)	\
	registerTestCase(static_cast<TestFunction>(&func), #func)

#define addTestWithName(func, name)	\
	registerTestCase(static_cast<TestFunction>(&func), name)

    void registerTestCase(TestFunction func, const std::string& name);
    bool check(long expected, long actual, const std::string& file, long line);

protected:
    std::string testName;

private:
    typedef std::vector<TestCase> TestCaseRegistry;

    TestCaseRegistry registry;
};

class Testbench {
public:
    static void addTestSuite(TestSuite *testSuite);
    static void runAllTestSuites();
    static void report(const std::string& name, const Source& source);

private:
    static Testbench& instance();

    void add(TestSuite *testSuite);
    void run();

private:
    static std::unique_ptr<TestResult> collector;

    typedef std::vector<TestSuite *> TestSuiteRegistry;
    TestSuiteRegistry testSuites;
};

#ifndef __FILENAME__
#define __FILENAME__                                                   \
(::strrchr(__FILE__,'/') ? ::strrchr(__FILE__,'/') + 1 : __FILE__)
#endif

#define TESTCASE(TestName)                              \
class TestName##TestCase : public Test::TestSuite {     \
public:                                                 \
    TestName##TestCase()                                \
        : TestSuite(#TestName)                          \
    {                                                   \
        addTestWithName(TestName##TestCase::standalone, #TestName); \
    }                                                   \
    void standalone();                                  \
} TestName##TestCase##Instance;                         \
void TestName##TestCase::standalone()

#define TEST_CHECK(condition)                           \
{                                                       \
    if (!(condition)) {                                 \
        Test::Testbench::report(testName,               \
        Test::Source(__FILENAME__, __LINE__, #condition));  \
        return;                                         \
    }                                                   \
}

#define TEST_EXPECT(expected, actual)                   \
{                                                       \
    __typeof__ (expected) _exp = (expected);            \
    __typeof__ (actual) _act = (actual);                \
    if (_exp != _act) {                                 \
        std::stringstream _stream;                      \
        _stream << "expected " << _exp                  \
                << " but it was " << _act;              \
        Test::Testbench::report(testName,               \
        Test::Source(__FILENAME__, __LINE__, _stream.str())); \
        return;                                         \
    }                                                   \
}

#define TEST_FAIL(text)                                 \
{                                                       \
    Test::Testbench::report(testName,                   \
    Test::Source(__FILENAME__, __LINE__, (text)));      \
    return;                                             \
}
} // namespace Test
#endif //!__TESTBENCH__
