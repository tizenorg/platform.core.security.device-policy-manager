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

#include "dbus/variant.h"

namespace dbus {

Variant::Variant(GVariant* var) :
    variant(var)
{
}

Variant::Variant(Variant&& var) :
    variant(var.variant)
{
    var.variant = nullptr;
}

Variant::Variant() :
    variant(nullptr)
{
}

Variant::~Variant()
{
    if (variant) {
        g_variant_unref(variant);
    }
}

Variant& Variant::operator=(GVariant* var)
{
    variant = var;
    return *this;
}

Variant::operator bool () const
{
    return variant != nullptr;
}

void Variant::get(const std::string& format, ...) const
{
    va_list ap;

    va_start(ap, format);
    g_variant_get_va(variant, format.c_str(), NULL, &ap);
    va_end(ap);
}

} // namespace dbus
