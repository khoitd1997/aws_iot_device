#ifndef _HANDLER_CONFIG_HPP
#define _HANDLER_CONFIG_HPP

#include "parent_handler.hpp"
#include "power_ctrl_handler.hpp"

#define SUBSCRIBE_TOPIC "/bedRoomLightReq"
#define PUBLISH_TOPIC "/bedRoomLightRes"

/****************Pin Config*****************************/
#define LIGHT_CTRL_PIN 21  // marked P21 on the board

// source: https://forum.mongoose-os.com/discussion/1218/unable-to-read-adc-value
#define ANALOG_PIN 17

#ifdef __cplusplus

/****************Device Handler Config*****************/

/**
 * @brief change this if you use more or less handler
 *
 */
#define TOTAL_HANDLER 1

/**
 * @brief empty since we don't use any interrupts
 *
 */
#define REGISTER_INTERRUPT() \
  {}

/**
 * @brief change this function to customize the list of handler
 *
 */
#define INITIALIZE_HANDLER()             \
  static PowerCtrlHandler pwrController; \
  static ParentHandler*   _handlerList[TOTAL_HANDLER] = {&pwrController};

#endif