#ifndef _HANDLER_CONFIG_HPP
#define _HANDLER_CONFIG_HPP

#include "parent_handler.hpp"
#include "power_ctrl_handler.hpp"
#include "temp_sensor_handler.hpp"

#define SUBSCRIBE_TOPIC "/pcReq"
#define PUBLISH_TOPIC "/pcRes"

/****************Device Handler Config*****************/

/**
 * @brief change this if you use more or less handler
 *
 */
#define TOTAL_HANDLER 2

/**
 * @brief change this function to customize the list of handler
 *
 */
#define INITIALIZE_HANDLER()              \
  static PowerCtrlHandler  pwrController; \
  static TempSensorHandler tempHandler;   \
  static ParentHandler*    _handlerList[TOTAL_HANDLER] = {&pwrController, &tempHandler};

#endif