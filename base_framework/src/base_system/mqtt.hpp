/**
 * @brief header file for mqtt functionalities
 *
 * @file mqtt.hpp
 * @author Khoi Trinh
 * @date 2018-09-01
 */

#ifndef _MQTT_UTIL_H_
#define _MQTT_UTIL_H_

#include "mgos.h"
#include "mgos_mqtt.h"

void sub(struct mg_connection *c, const char *fmt, ...);
void pub(struct mg_connection *c, const char *pubTopic, const char *fmt, ...);

#endif