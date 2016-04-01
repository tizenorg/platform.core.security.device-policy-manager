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

#ifndef __ZONE_POLICY__
#define __ZONE_POLICY__

#include "data-type.h"
#include "policy-context.hxx"

namespace DevicePolicyManager {

/**
 * This class provides APIs to create/remove and manage zones.
 */

class ZonePolicy {
public:
    ZonePolicy(PolicyControlContext& ctxt);
    ~ZonePolicy();

    int createZonePolicy(const std::string& name, const std::string& setupWizardAppid);
    int removeZonePolicy(const std::string& name);
    int lockZonePolicy(const std::string& name);
    int unlockZonePolicy(const std::string& name);

    std::vector<std::string> getZonePolicyList();

    int getZonePolicyState(const std::string& name);

private:
    PolicyControlContext& context;
};

} // namespace DevicePolicyManager
#endif // __ZONE_POLICY__
