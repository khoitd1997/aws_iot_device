#ifndef _MASTER_HANDLER_HPP
#define _MASTER_HANDLER_HPP

#include "parent_handler.hpp"
#include "power_ctrl_handler.hpp"
#include "temp_sensor_handler.hpp"

#include "event_handler_util.hpp"

#include "frozen.h"
#include "mgos.h"
#include "mgos_mqtt.h"

#define TOTAL_HANDLER 2  // REMEMBER TO CHANGE THIS

#define MAX_PAYLOAD_SIZE 150
#define MAX_RESPONSE_LEN 1500
/* Error Message Area */

class MasterAwsHandler {
 public:
  MasterAwsHandler();
  void handleAwsRequest(struct mg_connection* mgCon, struct mg_str* payload, char* returnMessage);
  static void handleAllReq(struct mg_connection* mgCon,
                           int                   eventType,
                           void*                 rawMessage,
                           void*                 user_data);

  static const char* getSubTopic(void);
  static const char* getPubTopic(void);

 private:
  // add items here to add more handler to your code
  PowerCtrlHandler  _powerCtrlHandler;
  TempSensorHandler _tempSensorHandler;

  // pub and sub topics
  static const char* _subTopic;
  static const char* _pubTopic;

  // arrays carrying all items
  ParentHandler* _handlerList[TOTAL_HANDLER];
};

#endif