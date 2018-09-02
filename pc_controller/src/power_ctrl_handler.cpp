/**
 * @brief source files for the power controller for the pc controller
 *
 * @file power_ctrl_handler.cpp
 * @author Khoi Trinh
 * @date 2018-09-01
 */

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

/**
 * @brief used to actuate a switch in the PC motherboard to short the powerOn pins, which will cause
 * the pc to either turn on(if it's off) or vice versa
 * @return HandlerError
 */
HandlerError PowerCtrlHandler::actuatePcChange(void) {
  write_pin(PC_CTRL_PIN, SWITCH_ON_STATE);
  mgos_msleep(SWITCH_DELAY_MS);  // delay to make sure the switch has effect
  write_pin(PC_CTRL_PIN, !SWITCH_ON_STATE);
  return HANDLER_NO_ERR;
}

/**
 * @brief handle the case power on switch interrupt
 *
 * @param pin pin that the interrupt comes from
 * @param arg any custom arguments that is to be passed to the interrupt handler, not used in
 * This case When the user presses the power on swithc on the case, the interrupt will be called to
 * actuate on that switch as well as update the status of the pc
 */
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

/**
 * @brief handle request pertaining to power control endpoint for the pc controller
 *
 * @param mgCon struct representing connection to server
 * @param message message sent to the mcu by the server
 * @param commandName name of the command in the payload sent by the server
 * @param response buffer to write the response to
 * @return HandlerError
 * Will read the command name and see whether to turn the pc on or off, then if the pc is not at the
 * desired state, actuate the change then make a report of the new state to be sent back to the
 * server
 */
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

/**
 * @brief setter for the pc status variable
 * @param pcStatus the status to change the variable to
 */
void PowerCtrlHandler::setPcStatus(bool pcStatus) { PowerCtrlHandler::_pcIsOn = pcStatus; }

/**
 * @brief used to get whether the pc is on or off
 * @return true pc is on
 * @return false pc is off
 * read the state variable to get the lastknown status of the pc
 */
bool PowerCtrlHandler::getPcStatus(void) { return PowerCtrlHandler::_pcIsOn; }

/**
 * @brief handle reports pertaining to power controller endpoint for pc controller
 * @param stateReport buffer to write report to
 * @return HandlerError
 * Will report the current PC is on or off
 */
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