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

#ifndef __DPM_SYSPOPUP_H__
#define __DPM_SYSPOPUP_H__

#include <syspopup_caller.h>

#include <string>

#include "exception.h"
#include "app-bundle.h"

class Syspopup {
public:
    Syspopup(const std::string& popup);
    Syspopup(const Syspopup&) = delete;
    Syspopup(Syspopup&&) = delete;

    int launch();
    int launch(const Bundle& bundle);

private:
    std::string name;
};

#endif //__DPM_SYSPOPUP_H__
