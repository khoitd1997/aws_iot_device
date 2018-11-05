/** @addtogroup bed_room_light_controller 
* 
* @{ 
*/ 

/**
 * @brief file storing the config for the bed room light controller
 *
 * @file device_config.hpp
 * @author Khoi Trinh
 * @date 2018-09-01
 */

#ifndef _HANDLER_CONFIG_HPP
#define _HANDLER_CONFIG_HPP

#include "parent_handler.hpp"
#include "power_ctrl_handler.hpp"

/****************MQTT Config****************************/

/**
 * @brief topic that the bed room light controller subscribe to
 */
#define SUBSCRIBE_TOPIC "/bedRoomLightReq"

/**
 * @brief topic that the bed room light controller will publish replies to
 */
#define PUBLISH_TOPIC "/bedRoomLightRes"

/****************Pin Config*****************************/

/**
 * @brief pins used for controlling the relay to turn on/off the light
 */
#define LIGHT_CTRL_PIN 21  // marked P21 on the board

/****************Device Handler Config*****************/
/**
 * @brief change this if you use more or less handler
 */
#define TOTAL_HANDLER 1

/**
 * @brief empty since we don't use any interrupts
 */
#define REGISTER_INTERRUPT() \
  {}

/**
 * @brief change this function to customize the list of handler
 */
#define INITIALIZE_HANDLER()             \
  static PowerCtrlHandler pwrController; \
  static ParentHandler*   _handlerList[TOTAL_HANDLER] = {&pwrController};

#endif

/** @} */
