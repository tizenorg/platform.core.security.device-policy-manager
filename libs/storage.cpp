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

#include "policy-client.h"

#include "storage.hxx"

namespace DevicePolicyManager {

StoragePolicy::StoragePolicy(PolicyControlContext& ctx) :
    context(ctx)
{
}

StoragePolicy::~StoragePolicy()
{
}

int StoragePolicy::setUsbMassStorageRestriction(bool enable)
{
	try {
		return context->methodCall<int>("StoragePolicy::setUsbMassStorageRestriction", enable);
	} catch (runtime::Exception& e) {
		return -1;
	}
}

bool StoragePolicy::isUsbMassStorageRestricted()
{
	try {
		return context->methodCall<bool>("StoragePolicy::isUsbMassStorageRestricted");
	} catch (runtime::Exception& e) {
		return -1;
	}
}

int StoragePolicy::wipeData()
{
	try {
		return context->methodCall<int>("StoragePolicy::wipeData");
	} catch (runtime::Exception& e) {
		return -1;
	}
}


} //namespace DevicePolicyManager
