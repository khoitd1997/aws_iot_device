#!/usr/bin/env bash
# used for setting up the dev board with necessary info

#----------BUILD CONFIG VARIABLE SECTION-------------
# bed_room_light_controller
# pc_controller
build_dir="pc_controller/" # change based on the devices you want to build

#----------DEPENDENCY SECTION------------------------
source ${build_dir}/credentials/mongoose_info.sh # get device info
source system_info/system_info.sh # get system info
set -e
set -x

#----------BUILD AND FLASH SECTION-------------------
cd ${build_dir}
mos build --arch ${arch} --clean
mos flash --timeout 10m
mos wifi ${guest_wifi_ssid} ${gues_wifi_password}

#----------CONFIGURE SECTION-------------------------

sleep 2 # fix for esp32
mos config-set device.password=${device_password} \
mqtt.server=${mqtt_server} --timeout 10m
mos console