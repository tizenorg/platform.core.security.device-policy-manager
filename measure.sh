#!/bin/bash

lcov -c -d device-policy-manager-0.0.1/server/CMakeFiles/device-policy-manager.dir	\
        -d device-policy-manager-0.0.1/libs/CMakeFiles/dpm.dir	\
        -d device-policy-manager-0.0.1/libs/CMakeFiles/dpm.dir/dpm	\
        -o info
genhtml info -o out
