#!/usr/bin/env bash
# used for intializing a new project structure

DIR_LIST="credentials fs src"
SKELETON_DIR="pc_controller"
set -x
set -e
#---------------------------------------------------------

if [ "$#" -ne 1 ]
then
  echo "Please supply the name of the device"
  exit 1
fi

PROJECT_NAME=$1

# creating directories
mkdir ${PROJECT_NAME}
cd ${PROJECT_NAME}
mkdir ${DIR_LIST}
cd ..

# copying basic files
cp ${SKELETON_DIR}/fs/index.html ${PROJECT_NAME}/fs/
cp ${SKELETON_DIR}/src/device_config.hpp ${PROJECT_NAME}/src/
cp ${SKELETON_DIR}/src/polling_function.cpp ${PROJECT_NAME}/src/polling_function.cpp
cp ${SKELETON_DIR}/mos.yml ${PROJECT_NAME}/
mkdir ${PROJECT_NAME}/credentials
cp sample/mongoose_info.sh ${PROJECT_NAME}/credentials/mongoose_info.sh

# first time initialization of the system info directory
if [ ! -d "system_info" ]; then
  mkdir system_info
  cp sample/system_info.sh system_info/system_info.sh
fi