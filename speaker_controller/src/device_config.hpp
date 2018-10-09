/**
 * @brief config file for the pc controller device
 *
 * @file device_config.hpp
 * @author Khoi Trinh
 * @date 2018-09-01
 */

#ifndef _HANDLER_CONFIG_HPP
#define _HANDLER_CONFIG_HPP

#include "parent_handler.hpp"
#include "speaker_ctrl_handler.hpp"

/****************MQTT Config****************************/
/**
 * @brief topic that the speaker controller subscribe to
 */
#define SUBSCRIBE_TOPIC "/speakerReq"
/**
 * @brief topic that the speaker controller will publish replies to
 */
#define PUBLISH_TOPIC "/speakerRes"

/****************Pinout Config**************************/

/**
 * @brief pins used to control the relay controlling the pc motherboard pins
 */
#define SPKR_PWR_PIN 14  // control speaker power source
#define SPKR_PWR_ON_STATE 1

#define SPKR_INC_PIN 5
#define SPKR_DIR_PIN 4
#define SPKR_SELECT_PIN 12

#define VOLUME_UP_LIMIT 100
#define VOLUME_DOWN_LIMIT 0

#define SPKR_VOLUME_DELAY_MS 1
#define SPKR_INC_ACTIVATED_STATE 0
#define SPKR_PIN_UP_STATE 1
#define SPKR_SELECT_ACTIVATED 0

// user input pins
#define SPKR_VOL_LEVEL_INPUT 0  // analog input
#define SPKR_PWR_INPUT_PIN 13
#define SPKR_PWR_INPUT_OFF_STATE 0
#define SPKR_PWR_INPUT_PIN_DEBOUNCE_MS 50

// TODO: add support for manual button

/****************Device Handler Config*****************/

/**
 * @brief change this if you use more or less handler
 *
 */
#define TOTAL_HANDLER 1

/**
 * @brief Reigister pc controller interrupt
 *
 * Register a button interrupt to actuate on pc's case power button
 */
#define REGISTER_INTERRUPT() \
  do {                       \
  } while (0)

/**
 * @brief change this function to customize the list of handler
 *
 */
#define INITIALIZE_HANDLER()                \
  static SpeakerCtrlHandler spkrController; \
  static ParentHandler*     _handlerList[TOTAL_HANDLER] = {&spkrController};

#endif