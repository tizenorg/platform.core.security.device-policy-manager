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

#include "storage.hxx"

#include "exception.h"
#include "audit/logger.h"

namespace DevicePolicyManager {

StoragePolicy::StoragePolicy(PolicyControlContext& ctx) :
    context(ctx)
{
	context.registerParametricMethod(this, (int)(StoragePolicy::setUsbMassStorageRestriction)(bool));
	context.registerNonparametricMethod(this, (bool)(StoragePolicy::isUsbMassStorageRestricted));
	context.registerNonparametricMethod(this, (bool)(StoragePolicy::wipeData));
}

StoragePolicy::~StoragePolicy()
{
}

int StoragePolicy::setUsbMassStorageRestriction(bool enable)
{
	return 0;
}

bool StoragePolicy::isUsbMassStorageRestricted()
{
	return true;
}

int StoragePolicy::wipeData()
{
	return 0;
}

StoragePolicy storagePolicy(Server::instance());

} //namespace DevicePolicyManager
