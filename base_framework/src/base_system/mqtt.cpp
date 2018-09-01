/**
 * @brief mqtt wrappers from mongoose, source:
 * https://github.com/cesanta/mongoose-os/blob/master/fw/examples/c_mqtt/src/main.c
 *
 * @file mqtt.c
 * @date 2018-08-05
 */

#include "mqtt.hpp"

#include "event_handler_util.hpp"
#include "master_handler.hpp"

#include "frozen/frozen.h"
#include "mgos.h"
#include "mgos_mqtt.h"

#include <stdint.h>
#include <stdio.h>

#define SUB_QOS 1
#define PUB_QOS 1

void sub(struct mg_connection *mgConnection, const char *fmt, ...) {
  char                            buf[MAX_RESPONSE_LEN];
  struct mg_mqtt_topic_expression topicExpression = {.topic = buf, .qos = SUB_QOS};

  // keep track of package ID through a static var in the function
  uint16_t sub_id = mgos_mqtt_get_packet_id();

  // retrieve argument list since this is a variable argument function
  va_list argList;
  va_start(argList, fmt);
  vsnprintf(buf, sizeof(buf), fmt, argList);  // create a string of argument to print later
  va_end(argList);

  mg_mqtt_subscribe(mgConnection, &topicExpression, 1, sub_id);
  LOG(LL_INFO, ("Subscribing to %s (id %u)", buf, sub_id));
}

void pub(struct mg_connection *mgConnection, const char *pubTopic, const char *fmt, ...) {
  char            msg[MAX_RESPONSE_LEN];
  struct json_out jmo = JSON_OUT_BUF(msg, sizeof(msg));
  va_list         argList;
  int             jsonMessageLength;

  // get the list of arguments and then create a json message as well as obtain their length
  va_start(argList, fmt);
  jsonMessageLength = json_vprintf(&jmo, fmt, argList);
  va_end(argList);

  mg_mqtt_publish(mgConnection,
                  pubTopic,
                  mgos_mqtt_get_packet_id(),
                  MG_MQTT_QOS(PUB_QOS),
                  msg,
                  jsonMessageLength);
  LOG(LL_INFO, ("%s -> %s", pubTopic, msg));
}