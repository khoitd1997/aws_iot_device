/**
 * @brief source files for the power control endpoint handler for the bed room light controller
 *
 * @file power_ctrl_handler.cpp
 * @author Khoi Trinh
 * @date 2018-09-01
 */

#include "power_ctrl_handler.hpp"

#include "event_handler_util.hpp"

#include "aws_iot_fmt.hpp"
#include "device_config.hpp"

#include <stdarg.h>
#include <cstring>

#include "frozen.h"
#include "mgos.h"
#include "mgos_gpio.h"
#include "mgos_mqtt.h"
#include "mgos_system.h"

bool PowerCtrlHandler::_lightIsOn = false;

/**
 * @brief Construct a new Power Ctrl Handler:: Power Ctrl Handler object intialize a power
 * controller handler for the bed room light controller
 *
 */
PowerCtrlHandler::PowerCtrlHandler(void) {
  strcpy(ParentHandler::_nameSpace, "Alexa.PowerController");
}

/**
 * @brief implement the handle request function for the power control endpoint for the bedroom light
 * controller
 *
 * @param mgCon struct representing connection to server
 * @param message message sent to the mcu by the server
 * @param commandName
 * @param response
 * @return HandlerError
 */
HandlerError PowerCtrlHandler::handleRequest(struct mg_connection* mgCon,
                                             struct mg_str*        message,
                                             char*                 commandName,
                                             char*                 response) {
  if (0 != strcmp(commandName, "TurnOff") && (0 != strcmp(commandName, "TurnOn"))) {
    return MQTT_ERR_UNKNOWN_COMMAND;
  }

  uint8_t pinState = (PowerCtrlHandler::getLightStatus() ? 1 : 0);
  if (0 == strcmp(commandName, "TurnOff") && (true == PowerCtrlHandler::getLightStatus())) {
    PowerCtrlHandler::setLightStatus(false);
    pinState = 0;
  } else if ((0 == strcmp(commandName, "TurnOn")) &&
             (false == PowerCtrlHandler::getLightStatus())) {
    PowerCtrlHandler::setLightStatus(true);
    pinState = 1;
  }
  write_pin(LIGHT_CTRL_PIN, pinState);

  char powerStateStr[5] = "";
  if (true == PowerCtrlHandler::getLightStatus()) {
    strcpy(powerStateStr, "ON");
  } else {
    strcpy(powerStateStr, "OFF");
  }

  createReport(response,
               PWR_CTRL_FMT,
               PWR_CTRL_TOTAL_ARG,
               "name",
               "powerState",
               "namespace",
               _nameSpace,
               "value",
               powerStateStr);

  (void)mgCon;
  (void)message;  // satisfy warning
  return HANDLER_NO_ERR;
}

/**
 * @brief
 *
 * @param lightStatus
 */
void PowerCtrlHandler::setLightStatus(bool lightStatus) {
  PowerCtrlHandler::_lightIsOn = lightStatus;
}

/**
 * @brief
 *
 * @return true
 * @return false
 */
bool PowerCtrlHandler::getLightStatus(void) { return PowerCtrlHandler::_lightIsOn; }

HandlerError PowerCtrlHandler::handleReport(char* stateReport) {
  char powerStateStr[5] = "";
  if (true == PowerCtrlHandler::getLightStatus()) {
    strcpy(powerStateStr, "ON");
  } else {
    strcpy(powerStateStr, "OFF");
  }

  return createReport(stateReport,
                      PWR_CTRL_FMT,
                      PWR_CTRL_TOTAL_ARG,
                      "name",
                      "powerState",
                      "namespace",
                      _nameSpace,
                      "value",
                      powerStateStr);
}