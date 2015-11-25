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

#ifndef __DPM_POLICY__
#define __DPM_POLICY__

#include <memory>
#include <stdexcept>
#include <string>

#include "xml/document.hxx"
#include "xml/node.hxx"

namespace DevicePolicyServer {

class Policy {
public:
    enum class Status : int {
        Off,
        On
    };

    enum class Retry : int {
        Default,
        Low,
        Medium,
        High
    };

    enum class Priority : int {
        Default,
        Low,
        Medium,
        High
    };

    Policy(const Policy&) = delete;
    Policy& operator=(const Policy&) = delete;

    Status getStatus();
    void setStatus(Status status);

    Priority getPriority();
    void setPriority(Priority priority);

    Retry getRetry();
    void setRetry(Retry retry);

private:
    Xml::Node data;
};

} // namespace DevicePolicyServer
#endif //__DPM_POLICY__
