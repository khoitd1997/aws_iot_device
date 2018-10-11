/**commandName
 * @brief header file for power controller class for bed room light controller
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
 * @brief power controller class for the bed room light controller, responsible for handling an
 * requests pertaining to the power controller namespace
 */
class PowerCtrlHandler : public ParentHandler {
 public:
  HandlerError handleRequest(struct mg_connection* mgCon,
                             const struct mg_str*  message,
                             const char*           commandName,
                             char*                 response) noexcept;
  HandlerError handleReport(char* stateReport) noexcept;
  ~PowerCtrlHandler(void){};
  PowerCtrlHandler(void);
  static void        setLightStatus(bool lightStatus) noexcept;
  static const bool& getLightStatus(void) noexcept;

 private:
  /**
   * @brief state variable to keep track of the last known light status
   */
  static bool _lightIsOn;
};

#endif