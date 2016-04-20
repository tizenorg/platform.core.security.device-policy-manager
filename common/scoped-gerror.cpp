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

#include "scoped-gerror.h"

 ScopedGError::ScopedGError() :
     error(nullptr)
 {
 }

 ScopedGError::~ScopedGError()
 {
     if (error) {
         g_error_free(error);
     }
 }

 GError** ScopedGError::operator& ()
 {
     return &error;
 }

 const GError* ScopedGError::operator-> () const
 {
         return error;
 }

ScopedGError::operator bool () const
{
    return error != nullptr;
}

 std::ostream& operator<<(std::ostream& os, const ScopedGError& e)
 {
     os << e->message;
 }
