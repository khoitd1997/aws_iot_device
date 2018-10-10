#!/usr/bin/env bash
# used for generating doxygen docs

# add more if there is new devices
PROJECT_DIR_LIST="base_framework/ bed_room_light_controller/ pc_controller/ speaker_controller/"
set -x
set -e
#-------------------------------------------------------


for project in ${PROJECT_DIR_LIST}; do
    cd ${project}
    doxygen Doxyfile
    cd ..
done
