#!/bin/bash

lcov -c -d device-policy-manager-0.0.1/server/CMakeFiles/device-policy-manager.dir	\
        -d device-policy-manager-0.0.1/libs/CMakeFiles/dpm.dir	\
        -d device-policy-manager-0.0.1/libs/CMakeFiles/dpm.dir/dpm	\
        -d device-policy-manager-0.0.1/common/CMakeFiles/dpm-common.dir	\
        -d device-policy-manager-0.0.1/common/CMakeFiles/dpm-common.dir/audit	\
        -d device-policy-manager-0.0.1/common/CMakeFiles/dpm-common.dir/db	\
        -d device-policy-manager-0.0.1/common/CMakeFiles/dpm-common.dir/auth	\
        -d device-policy-manager-0.0.1/common/CMakeFiles/dpm-common.dir/xml	\
        -d device-policy-manager-0.0.1/common/CMakeFiles/dpm-common.dir/dbus	\
        -d device-policy-manager-0.0.1/common/CMakeFiles/dpm-common.dir/rmi	\
        -o info
genhtml info -o out
