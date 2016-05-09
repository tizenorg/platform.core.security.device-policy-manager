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
SET(CML_TOOLKIT_TARGET	"dpm-cml-toolkit")

SET(CML_TOOLKIT_SOURCES main.c
    dpm-cml-toolkit.c
    bluetooth.c
    password.c
    restriction.c
    zone.c
)

ADD_EXECUTABLE(${CML_TOOLKIT_TARGET} ${CML_TOOLKIT_SOURCES})

INCLUDE_DIRECTORIES(SYSTEM ${DPM_LIBS} ${DPM_TOOLS})

TARGET_LINK_LIBRARIES(${CML_TOOLKIT_TARGET} dpm)

INSTALL(TARGETS ${CML_TOOLKIT_TARGET} DESTINATION bin)
