#!/usr/bin/env bash
# used for setting up the dev board with necessary info

#----------DEPENDENCY SECTION------------------------
source credentials/mongoose_info.sh # get necessary info
set -e
set -x

#----------BUILD AND FLASH SECTION-------------------
mos build --arch ${arch} --clean
mos flash
mos wifi ${guest_wifi_ssid} ${gues_wifi_password}

#----------CONFIGURE SECTION-------------------------

mos config-set device.password=${device_password} \
mqtt.server=${mqtt_server}
mos console