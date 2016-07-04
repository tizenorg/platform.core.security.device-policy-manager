#
# Copyright (c) 2016 Samsung Electronics Co., Ltd All Rights Reserved
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

SET(ZONE_MODULE		  ${DPM_ZONE}/module)
SET(ZONE_LIBS		  ${DPM_ZONE}/libs)
SET(ZONE_CLI		  ${DPM_ZONE}/cli)
SET(ZONE_VOLUME           ${DPM_ZONE}/volume)

ADD_SUBDIRECTORY(${ZONE_MODULE})
ADD_SUBDIRECTORY(${ZONE_LIBS})
ADD_SUBDIRECTORY(${ZONE_CLI})
ADD_SUBDIRECTORY(${ZONE_VOLUME})

INSTALL(FILES gumd-scripts/20_pam-zone-add.post DESTINATION /etc/gumd/useradd.d)
INSTALL(FILES gumd-scripts/20_pam-zone-remove.post DESTINATION /etc/gumd/userdel.d)
