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

#ifndef __REFLECTION__
#define __REFLECTION__

#include "preprocessor.hxx"

#define VISIT_ELEMENT(elem) v.visit(#elem, elem)

#define REFLECTABLE(...)				\
template<typename V>					\
void accept(V v)					\
{							\
	FOR_EACH_VAR_ARGS(VISIT_ELEMENT, __VA_ARGS__);	\
}							\
template<typename V>					\
void accept(V v) const					\
{							\
	FOR_EACH_VAR_ARGS(VISIT_ELEMENT, __VA_ARGS__);	\
}

#define NO_REFLECTABLE_PROPERTY				\
	template<typename V>				\
	static void accept(V) {}

#endif //!__REFLECTION__
