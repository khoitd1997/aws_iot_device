
#include "master_handler.hpp"

#include "handler_config.hpp"
#include "power_ctrl_handler.hpp"
#include "temp_sensor_handler.hpp"

#include "event_handler_util.hpp"

#include "frozen.h"
#include "mgos.h"
#include "mgos_mqtt.h"
#include "mqtt.hpp"

#include <stdint.h>
#include <cstring>

#define HANDLER_TRY_FUNC(funcToExecute)                \
  do {                                                 \
    HandlerError errCode;                              \
    errCode = funcToExecute;                           \
    if (errCode != HANDLER_NO_ERR) {                   \
      create_error_message(errCode, payload, jsonBuf); \
      return;                                          \
    }                                                  \
  } while (0)

const char* MasterAwsHandler::_subTopic = "/pcReq";
const char* MasterAwsHandler::_pubTopic = "/pcRes";

// modify this to add more handler
MasterAwsHandler::MasterAwsHandler() {
  for (int loopIndex = 0; loopIndex < TOTAL_HANDLER; ++loopIndex) {
    _handlerList[loopIndex] = NULL;
  }

  _handlerList[0] = &_powerCtrlHandler;
  _handlerList[1] = &_tempSensorHandler;
}

void MasterAwsHandler::handleAllReq(struct mg_connection* mgCon,
                                    int                   eventType,
                                    void*                 rawMessage,
                                    void*                 user_data) {
  struct mg_mqtt_message* msg                       = (struct mg_mqtt_message*)rawMessage;
  struct mg_str*          payload                   = &msg->payload;
  char                    jsonBuf[MAX_RESPONSE_LEN] = "";
  static MasterAwsHandler masterHandler;

  switch (eventType) {
    case MG_EV_MQTT_CONNACK:
      LOG(LL_INFO, ("CONNACK: %d", msg->connack_ret_code));
      if (MasterAwsHandler::getSubTopic() == NULL || MasterAwsHandler::getPubTopic() == NULL) {
        LOG(LL_ERROR, ("Run 'mgos config-set mqtt.sub=... mqtt.pub=...'"));
        create_error_message(MQTT_CONFIG_NOT_SET, payload, jsonBuf);
      } else {
        sub(mgCon,
            "%s",
            MasterAwsHandler::getSubTopic());  // subscribe to the pre-configured aws path
      }
      break;
    case MG_EV_POLL:
      // this event happens a lot, logging it would cause a lot of garbage in console
      break;
    case MG_EV_CLOSE:
      LOG(LL_INFO, ("Connection closed"));
      break;
    case MG_EV_MQTT_PUBACK:
      LOG(LL_INFO, ("Publication %u acknowledged", msg->message_id));
      break;

    case MG_EV_MQTT_SUBACK:
      LOG(LL_INFO, ("Subscription %u acknowledged", msg->message_id));
      break;

    case MG_EV_MQTT_PUBLISH:

      mg_mqtt_puback(mgCon, msg->message_id);  // acknowledge the publish from others
      masterHandler.handleAwsRequest(mgCon, payload, jsonBuf);
      break;

    default:
      LOG(LL_ERROR, ("Error: unkown event type %d", eventType));
      create_error_message(MQTT_ERR_UNKNOWN_COMMAND, payload, jsonBuf);
  }

  if (0 != strcmp(jsonBuf, "")) { pub(mgCon, masterHandler.getPubTopic(), "%s", jsonBuf); }

  (void)user_data;  // used to silence warning
}

void MasterAwsHandler::handleAwsRequest(struct mg_connection* mgCon,
                                        struct mg_str*        payload,
                                        char*                 jsonBuf) {
  for (int loopIndex = 0; loopIndex < TOTAL_HANDLER; ++loopIndex) {
    if (NULL == _handlerList[loopIndex]) {
      create_error_message(HANDLER_NULL, payload, jsonBuf);
      return;
    }
  }

  LOG(LL_INFO, ("Received Publication"));
  LOG(LL_INFO, ("got command: [%.*s]", (int)payload->len, payload->p));

  if (NULL == _handlerList[0]) {
    LOG(LL_INFO, ("Empty handler list"));
  } else {
    LOG(LL_INFO, ("Non-empty handler list"));
  }

  char commandName[MAX_COMMAND_LEN]     = "";
  char nameSpaceName[MAX_NAMESPACE_LEN] = "";
  HANDLER_TRY_FUNC(getCommandInfo(payload, commandName, nameSpaceName));
  LOG(LL_INFO, ("Done Parsing Command name: %s, namespace: %s", commandName, nameSpaceName));

  HandlerError handlerError = HANDLER_NO_ERR;

  if (HANDLER_NO_ERR == handlerError) {
    if (0 != strcmp(commandName, "ReportState")) {
      LOG(LL_INFO, ("Received a non report state command"));
      int loopIndex;
      for (loopIndex = 0; loopIndex < TOTAL_HANDLER; ++loopIndex) {
        LOG(LL_INFO, ("Looping through handler list"));
        if (NULL != _handlerList[loopIndex]) {
          LOG(LL_INFO,
              ("Checking handler with namespace %s", (_handlerList[loopIndex])->getNameSpace()));

          if (true == (_handlerList[loopIndex])->canHandleRequest(nameSpaceName)) {
            LOG(LL_INFO, ("Found Supported Handler"));
            printStateReportOpen(jsonBuf);
            handlerError =
                (_handlerList[loopIndex])->handleRequest(mgCon, payload, commandName, jsonBuf);
            printStateReportEnd(jsonBuf);
            break;
          }
        }
      }

      if (TOTAL_HANDLER == loopIndex) { handlerError = MQTT_ERR_UNKNOWN_COMMAND; }

    } else {
      printStateReportOpen(jsonBuf);

      for (int loopIndex = 0; loopIndex < TOTAL_HANDLER; ++loopIndex) {
        // TODO: add more error handling
        (_handlerList[loopIndex])->handleReport(jsonBuf);
        if (loopIndex < TOTAL_HANDLER - 1) { printStateReportDelimiter(jsonBuf); }
      }

      printStateReportEnd(jsonBuf);
    }
  }

  if (HANDLER_NO_ERR != handlerError) { create_error_message(handlerError, payload, jsonBuf); }
  // else {
  //   pub(mgCon, "%s", _pubTopic, jsonBuf);
  // }
}

const char* MasterAwsHandler::getSubTopic() { return _subTopic; }

const char* MasterAwsHandler::getPubTopic() { return _pubTopic; }