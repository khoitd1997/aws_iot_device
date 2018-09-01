#ifndef _HANDLER_CONFIG_HPP
#define _HANDLER_CONFIG_HPP

#include "parent_handler.hpp"
#include "power_ctrl_handler.hpp"

#define SUBSCRIBE_TOPIC "/bedRoomLightReq"
#define PUBLISH_TOPIC "/bedRoomLightRes"

/****************Device Handler Config*****************/

/**
 * @brief change this if you use more or less handler
 *
 */
#define TOTAL_HANDLER 1

/**
 * @brief change this function to customize the list of handler
 *
 */
#define INITIALIZE_HANDLER()             \
  static PowerCtrlHandler pwrController; \
  static ParentHandler*   _handlerList[TOTAL_HANDLER] = {&pwrController};

#endif