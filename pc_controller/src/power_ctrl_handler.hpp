/**
 * @brief
 *
 * @file power_ctrl_handler.hpp
 * @author Khoi Trinh
 * @date 2018-09-01
 */

#ifndef _POWER_CTRL_HANDLER_HPP
#define _POWER_CTRL_HANDLER_HPP

#include "parent_handler.hpp"

#include "event_handler_util.hpp"

#include "frozen.h"
#include "mgos.h"
#include "mgos_mqtt.h"

#define SWITCH_ON_STATE 1
#define BUTTON_DEBOUNCE_MS 50
#define SWITCH_DELAY_MS 150
/**
 * @brief
 *
 */
class PowerCtrlHandler : public ParentHandler {
 public:
  HandlerError handleRequest(struct mg_connection* mgCon,
                             struct mg_str*        message,
                             char*                 commandName,
                             char*                 response);
  HandlerError handleReport(char* stateReport);
  ~PowerCtrlHandler(void){};
  PowerCtrlHandler(void);
  static void buttonInterruptHandler(int pin, void* arg);
  static void setPcStatus(bool pcStatus);
  static bool getPcStatus(void);

 private:
  static HandlerError actuatePcChange(void);
  static bool         _pcIsOn;
};

#endif