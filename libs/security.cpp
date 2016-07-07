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

#include "security.hxx"
#include "audit/logger.h"

namespace DevicePolicyManager {

SecurityPolicy::SecurityPolicy(PolicyControlContext& ctxt) :
	context(ctxt)
{
}

SecurityPolicy::~SecurityPolicy()
{
}

int SecurityPolicy::lockoutScreen()
{
	try {
		return context->methodCall<int>("SecurityPolicy::lockoutScreen");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int SecurityPolicy::setInternalStorageEncryption(bool encrypt)
{
	try {
		return context->methodCall<int>("SecurityPolicy::setInternalStorageEncryption", encrypt);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int SecurityPolicy::isInternalStorageEncrypted()
{
	try {
		return context->methodCall<int>("SecurityPolicy::isInternalStorageEncrypted");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int SecurityPolicy::setExternalStorageEncryption(bool encrypt)
{
	try {
		return context->methodCall<int>("SecurityPolicy::setExternalStorageEncryption", encrypt);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int SecurityPolicy::isExternalStorageEncrypted()
{
	try {
		return context->methodCall<int>("SecurityPolicy::isExternalStorageEncrypted");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

} // namespace DevicePolicyManager
