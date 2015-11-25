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

#include <memory>
#include <stdexcept>
#include <string>

#include "policy.hxx"

namespace DevicePolicyServer {

Policy::Status Policy::getStatus()
{
    return Status::Off;
}

void Policy::setStatus(Policy::Status status)
{
}

Policy::Priority Policy::getPriority()
{
    return Priority::Default;
}

void Policy::setPriority(Policy::Priority priority)
{
}

Policy::Retry Policy::getRetry()
{
    return Retry::Default;
}

void Policy::setRetry(Policy::Retry retry)
{
}

} // namespace DevicePolicyServer
