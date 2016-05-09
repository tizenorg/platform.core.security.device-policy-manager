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

SET(DPM_CLI ${DPM_TOOLS}/cli)
SET(DPM_ODE ${DPM_TOOLS}/ode)
SET(DPM_ZONE_SETUP_WIZARD ${DPM_TOOLS}/zone-setup-wizard)
SET(DPM_SYSPOPUP ${DPM_TOOLS}/syspopup)
SET(DPM_TOOLKIT ${DPM_TOOLS}/dpm-cml-toolkit)

ADD_SUBDIRECTORY(${DPM_CLI})
ADD_SUBDIRECTORY(${DPM_ODE})
ADD_SUBDIRECTORY(${DPM_ZONE_SETUP_WIZARD})
ADD_SUBDIRECTORY(${DPM_SYSPOPUP})
ADD_SUBDIRECTORY(${DPM_TOOLKIT})
