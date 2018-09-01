#include "power_ctrl_handler.hpp"

#include "aws_iot_fmt.hpp"
#include "event_handler_util.hpp"

#include "device_config.hpp"

#include <stdarg.h>
#include <cstring>

#include "frozen.h"
#include "mgos.h"
#include "mgos_gpio.h"
#include "mgos_mqtt.h"
#include "mgos_system.h"

bool PowerCtrlHandler::_pcIsOn = false;

PowerCtrlHandler::PowerCtrlHandler(void) {
  strcpy(ParentHandler::_nameSpace, "Alexa.PowerController");
}

HandlerError PowerCtrlHandler::actuatePcChange(void) {
  write_pin(PC_CTRL_PIN, SWITCH_ON_STATE);
  mgos_msleep(SWITCH_DELAY_MS);  // delay to make sure the switch has effect
  write_pin(PC_CTRL_PIN, !SWITCH_ON_STATE);
  return HANDLER_NO_ERR;
}

void PowerCtrlHandler::buttonInterruptHandler(int pin, void* arg) {
  // LOG(LL_INFO, ("Button pressed"));
  if (PC_BUTTON_PIN == pin) {
    // LOG(LL_INFO, ("Current PC status: %d", PowerCtrlHandler::getPcStatus()));
    PowerCtrlHandler::setPcStatus(!PowerCtrlHandler::getPcStatus());
    LOG(LL_INFO, ("Changed PC status: %d", PowerCtrlHandler::getPcStatus()));
    PowerCtrlHandler::actuatePcChange();
  }
  (void)arg;
}

HandlerError PowerCtrlHandler::handleRequest(struct mg_connection* mgCon,
                                             struct mg_str*        message,
                                             char*                 commandName,
                                             char*                 response) {
  mgos_gpio_disable_int(PC_CTRL_PIN);

  if (0 != strcmp(commandName, "TurnOff") && (0 != strcmp(commandName, "TurnOn"))) {
    return MQTT_ERR_UNKNOWN_COMMAND;
  }

  if (0 == strcmp(commandName, "TurnOff") && (true == PowerCtrlHandler::getPcStatus())) {
    PowerCtrlHandler::setPcStatus(false);
    actuatePcChange();
  } else if ((0 == strcmp(commandName, "TurnOn")) && (false == PowerCtrlHandler::getPcStatus())) {
    PowerCtrlHandler::setPcStatus(true);
    actuatePcChange();
  }
  mgos_gpio_enable_int(PC_CTRL_PIN);

  char powerStateStr[5] = "";
  if (true == PowerCtrlHandler::getPcStatus()) {
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

void PowerCtrlHandler::setPcStatus(bool pcStatus) { PowerCtrlHandler::_pcIsOn = pcStatus; }
bool PowerCtrlHandler::getPcStatus(void) { return PowerCtrlHandler::_pcIsOn; }

HandlerError PowerCtrlHandler::handleReport(char* stateReport) {
  mgos_gpio_disable_int(PC_CTRL_PIN);
  char powerStateStr[5] = "";
  if (true == PowerCtrlHandler::getPcStatus()) {
    strcpy(powerStateStr, "ON");
  } else {
    strcpy(powerStateStr, "OFF");
  }
  mgos_gpio_enable_int(PC_CTRL_PIN);
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