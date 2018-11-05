

/** @addtogroup pc_controller
 *
 * @{
 */

/**
 * @brief header file for the power controller class of the pc controller
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

/**
 * @brief state of the pin when the relay is on
 */
#define SWITCH_ON_STATE 1

/**
 * @brief debouncing period for the pc case switch
 */
#define BUTTON_DEBOUNCE_MS 50

/**
 * @brief the delay between turning on and turning off the relay controlling the pc motherboard
 * pins, used to make sure that the switching actually has an observable effect
 *
 */
#define SWITCH_DELAY_MS 150

/**
 * @brief the power controller class responsible for handling power controller endpoint
 * functionalities for the pc controller
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

  /**
   * @brief state variable keeping track if the pc is on or off
   */
  static bool _pcIsOn;
};

#endif

/** @} */
