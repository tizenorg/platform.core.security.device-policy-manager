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

#ifndef __PROCESS__
#define __PROCESS__

#include <string>

namespace Runtime {

class Process {
public:
    typedef pid_t Pid;

    Process(const std::string& prog);
    Process(const Process& proc);

    ~Process();

    Process& operator=(const Process& proc);

    int execute();
    int start();
    int waitForFinished();
    int waitForStarted();
    void terminate();
    void kill();

    bool isRunning() const;

private:
    int status;
    Pid pid;
    std::string program;
};

} // namespace Runtime
#endif //__EXCEPTION__
