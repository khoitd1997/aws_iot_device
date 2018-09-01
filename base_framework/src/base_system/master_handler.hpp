#ifndef _MASTER_HANDLER_HPP
#define _MASTER_HANDLER_HPP

#include "event_handler_util.hpp"

#include "frozen.h"
#include "mgos.h"
#include "mgos_mqtt.h"

#define MAX_PAYLOAD_SIZE 150
#define MAX_RESPONSE_LEN 1500
/* Error Message Area */

class MasterAwsHandler {
 public:
  MasterAwsHandler();
  void handleAwsRequest(struct mg_connection *mgCon, struct mg_str *payload, char *returnMessage);
  static void handleAllReq(struct mg_connection *mgCon,
                           int                   eventType,
                           void *                rawMessage,
                           void *                user_data);

  static const char *getSubTopic(void);
  static const char *getPubTopic(void);

 private:
  // pub and sub topics
  static const char *_subTopic;
  static const char *_pubTopic;
};

#endif