#
# Copyright (c) 2015 Samsung Electronics Co., Ltd All Rights Reserved
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
SET(API_TEST_TARGET	"dpm-api-tests")

SET(FOUNDATION  main.c
                testbench.c
				context.c
)

SET(POLICY  restriction.c
            security.c
)

INCLUDE("${TIZEN_PROFILE_NAME}.cmake")

ADD_EXECUTABLE(${API_TEST_TARGET} ${FOUNDATION} ${POLICY})

INCLUDE_DIRECTORIES(SYSTEM ${DPM_LIBS} ${DPM_TESTS})

TARGET_LINK_LIBRARIES(${API_TEST_TARGET} dpm)

INSTALL(TARGETS ${API_TEST_TARGET} DESTINATION bin)
