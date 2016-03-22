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
#include <utility>

#include "data-type.h"
#include "rmi/service.h"
#include "rmi/client.h"
#include "audit/logger.h"

#include "testbench/testbench.h"

const std::string IPC_TEST_ADDRESS = "/tmp/.dpm-test";

class TestServer {
public:
    TestServer()
    {
        service.reset(new rmi::Service(IPC_TEST_ADDRESS));

        service->registerMethod(this, (String)(TestServer::method1)(String));
        service->registerMethod(this, (String)(TestServer::method2)(String, String));
        service->registerMethod(this, (String)(TestServer::method3)(String, String, String));
        service->registerMethod(this, (String)(TestServer::method4)(String, String, String, String));
    }

    void run()
    {
        service->start();
    }

    String method1(String& arg1)
    {
        INFO("[SERVER] Remote method1 is called with: " + arg1.value);
        return String("Method1 result");
    }

    String method2(String& arg1, String& arg2)
    {
        INFO("[SERVER] Remote method2 is called with: " + arg1.value + ", " + arg2.value);
        return String("Method2 result");
    }

    String method3(String& arg1, String& arg2, String& arg3)
    {
        INFO("[SERVER] Remote method3 is called with: " + arg1.value + ", " + arg2.value + ", " + arg3.value);
        return String("Method3 result");
    }

    String method4(String& arg1, String& arg2, String& arg3, String& arg4)
    {
        INFO("[SERVER] Remote method4 is called with: " + arg1.value + ", " + arg2.value + ", " + arg3.value + ", " + arg4.value);
        return String("Method4 result");
    }

private:
    std::unique_ptr<rmi::Service> service;
};

class TestClient {
public:
    TestClient()
    {
    }

    void connect()
    {
        client.reset(new rmi::Client(IPC_TEST_ADDRESS));
        client->connect();
    }

    void disconnect()
    {
        client.reset();
    }

    String method1(String& arg1)
    {
        return client->methodCall<String>("TestServer::method1", arg1);
    }

    String method2(String& arg1, String& arg2)
    {
        return client->methodCall<String>("TestServer::method2", arg1, arg2);
    }

    String method3(String& arg1, String& arg2, String& arg3)
    {
        return client->methodCall<String>("TestServer::method3", arg1, arg2, arg3);
    }

    String method4(String& arg1, String& arg2, String& arg3, String& arg4)
    {
        return client->methodCall<String>("TestServer::method4", arg1, arg2, arg3, arg4);
    }

    String invalidMethod(String& arg)
    {
        return client->methodCall<String>("TestServer::invalidMethod", arg);
    }

private:
    std::unique_ptr<rmi::Client> client;
};

int WaitForFile(const std::string& path, const unsigned int timeout)
{
    struct stat st;
    unsigned int loop = 0;

    while (stat(path.c_str(), &st) == -1) {
        if (errno != ENOENT) {
            ERROR("Error on waitting for: " + path);
            return -1;
        }

        if (((++loop) * 100) > timeout) {
            ERROR("Error on waitting for: " + path);
            return -1;
        }

        usleep(100 * 1000);
    }

    return 0;
}

int WaitForPid(pid_t pid)
{
    int status, ret;
    char errmsg[256];

    do {
        ret = waitpid(pid, &status, 0);
        if (ret == -1) {
            if (errno != EINTR) {
                ERROR("Wait Pid failed: " + std::to_string(pid) + "(" + strerror_r(errno, errmsg, sizeof(errmsg)) + ")");
                return -1;
            }
        }
    } while (ret == EINTR || ret != pid);

    return status;
}

pid_t PrepareTestServer(void)
{
    ::unlink(IPC_TEST_ADDRESS.c_str());

    pid_t pid = fork();

    if (pid < 0) {
        return -1;
    }

    if (pid == 0) {
        try {
            TestServer server;
            server.run();
        } catch (std::exception& e) {
            ERROR(e.what());
            return -1;
        }

        return 0;
    }

    return pid;
}

class IpcTestSuite : public testbench::TestSuite {
public:
    IpcTestSuite(const std::string& name) :
        testbench::TestSuite(name)
    {
        addTest(IpcTestSuite::connectionTest);
        addTest(IpcTestSuite::remoteMethodCallTest);
    }

    void setup()
    {
        pid = PrepareTestServer();
        if (pid == -1) {
            ERROR("Preparing test server failed");
            return;
        }

        WaitForFile(IPC_TEST_ADDRESS, 1000);
    }

    void teardown()
    {
        if (::kill(pid, SIGTERM) == -1) {
            return;
        }

        int status = WaitForPid(pid);
        if (status == -1 || !WIFEXITED(status)) {
            return;
        }
    }

    void connectionTest()
    {
        try {
            TestClient client;
            client.connect();
            client.disconnect();
        } catch (runtime::Exception& e) {
            ERROR(e.what());
        }
    }

    void remoteMethodCallTest()
    {
        try {
            TestClient client;
            client.connect();

            String param1("First Parameter");
            String param2("Second Parameter");
            String param3("Third Parameter");
            String param4("Fourth Parameter");

            String result1 = client.method1(param1);
            String result2 = client.method2(param1, param2);
            String result3 = client.method3(param1, param2, param3);
            String result4 = client.method4(param1, param2, param3, param4);

            client.disconnect();
        } catch (runtime::Exception& e) {
            ERROR(e.what());
        }
    }

private:
    pid_t pid;
};

IpcTestSuite ipcTestSuite("IpcTestSuite");
