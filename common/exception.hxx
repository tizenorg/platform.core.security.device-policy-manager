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

#ifndef __DPM_EXCEPTION__
#define __DPM_EXCEPTION__

#include <stdexcept>
#include <string>

namespace Runtime {

class Exception: public std::runtime_error {
public:
    Exception(const std::string& error, const unsigned int code = 0)
        : std::runtime_error(error), code(code) {
    }

    const unsigned int getCode() const {
        return code;
    }

private:
    const unsigned int code;
};
} // namespace Runtime
#endif //__EXCEPTION__
