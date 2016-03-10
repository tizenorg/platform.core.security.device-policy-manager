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
#include <unistd.h>
#include <aul.h>

#include "launchpad.h"

#include "error.h"
#include "exception.h"
#include "audit/logger.h"

Launchpad::Launchpad(const uid_t uid) :
    user(uid)
{
    if (!user) {
        user = ::getuid();
    }
}

int Launchpad::launch(const std::string& appid)
{
    return launch(appid, Bundle());
}

int Launchpad::launch(const std::string& appid, const Bundle& bundle)
{
    if (::aul_launch_app_for_uid(appid.c_str(), bundle.get(), user) < 0) {
        return -1;
    }

    return 0;
}
