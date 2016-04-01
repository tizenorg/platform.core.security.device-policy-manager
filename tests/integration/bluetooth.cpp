/*
 *  Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

#include "bluetooth.hxx"
#include "policy-client.h"
#include "testbench/testbench.h"
#include "audit/logger.h"

TESTCASE(DeviceRestriction)
{
    DevicePolicyContext client;
    int error = -1;
    bool status = false;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::BluetoothPolicy *bluetooth = client.createPolicyInterface<DevicePolicyManager::BluetoothPolicy>();

    error = bluetooth->setDeviceRestriction(true);
    TEST_EXPECT(0, error);

    status = bluetooth->isDeviceRestricted();
    // the 'status' will be always 'false' before server/bluetooth.cpp has been implemented.
    TEST_EXPECT(true, status);

    error = bluetooth->setDeviceRestriction(false);
    TEST_EXPECT(0, error);

    status = bluetooth->isDeviceRestricted();
    TEST_EXPECT(false, status);
}

TESTCASE(UuidRestriction)
{
    DevicePolicyContext client;
    int error = -1;
    bool status = false;

    TEST_EXPECT(0, client.connect());

    DevicePolicyManager::BluetoothPolicy *bluetooth = client.createPolicyInterface<DevicePolicyManager::BluetoothPolicy>();

    error = bluetooth->setUuidRestriction(true);
    TEST_EXPECT(0, error);

    status = bluetooth->isUuidRestricted();
    // the 'status' will be always 'false' before server/bluetooth.cpp has been implemented.
    TEST_EXPECT(true, status);

    error = bluetooth->setUuidRestriction(false);
    TEST_EXPECT(0, error);

    status = bluetooth->isUuidRestricted();
    TEST_EXPECT(false, status);
}
