/**
 * @brief config file for the speaker controller device
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
 * @brief pins for controlling the power source to the speaker, probably controlling through a relay
 * instead of directly to the speaker
 *
 */
#define SPKR_PWR_PIN 14

/**
 * @brief the state of the power control pin if the speaker is powered on
 *
 */
#define SPKR_PWR_ON_STATE 1

/**
 * @brief pins that interact witht the INC pin of the digital pot, negative edge on this pin will
 * move the digital pot wiper
 *
 */
#define SPKR_INC_PIN 5

/**
 * @brief pins that controls the DIR pin of the digital pot, used for controlling whether the
 * negative edge on the INC pin result in increase or decrease in resistance
 *
 */
#define SPKR_DIR_PIN 4

/**
 * @brief pin that controls the CS line of the digital pot, the digital pot will only changes its
 * resistance if the CS pin is active
 *
 */
#define SPKR_SELECT_PIN 12

/**
 * @brief Max possible volume set by the user
 *
 */
#define VOLUME_UP_LIMIT 100
/**
 * @brief Min possible volume set by the user
 *
 */
#define VOLUME_DOWN_LIMIT 0

/**
 * @brief delay between toggling the INC pin to make sure the edge is registered by the digital pot
 *
 */
#define SPKR_VOLUME_DELAY_MS 1

/**
 * @brief the state that the INC pin is activated, the pin triggers on negative edge so its
 * activated state is low
 *
 */
#define SPKR_INC_ACTIVATED_STATE 0

/**
 * @brief the state of the pin that controls the DIR pin of the digital pot when the pot is in
 * moving up resistance mode
 *
 */
#define SPKR_PIN_UP_STATE 1

/**
 * @brief the state of the pin that controls the CS pin of the digital pot when the pot is activated
 *
 */
#define SPKR_SELECT_ACTIVATED 0

/**
 * @brief pins for sensing analog voltage of the physical pot
 *
 */
#define SPKR_VOL_LEVEL_INPUT_PIN 0

/**
 * @brief minimum change in adc voltage before it's considered that the user moved the pot
 *
 */
#define SPKR_VOL_LEVEL_INPUT_THRESHOLD 15

/**
 * @brief max value returned by the adc read function of mongoose
 *
 */
#define SPKR_VOL_LEVEL_MAX_COUNT 1024

/**
 * @brief pins for sensing user turning on/off the speaker using the physical pot
 *
 */
#define SPKR_PWR_INPUT_PIN 13

/**
 * @brief the state the the input pin will be at if it's off, since it's pulled up so the off state
 * will be one
 *
 */
#define SPKR_PWR_INPUT_OFF_STATE 1

/****************Device Handler Config*****************/

/**
 * @brief dummy function, interrupts are registered in the handler constructor for this device
 *
 * Register an interrupt before handling any events, usually you would register interrupt inside the
 * constructor of the handler but this is provided to register interrupt early in the program if
 * needed
 */
#define REGISTER_INTERRUPT() \
  do {                       \
  } while (0)

/**
 * @brief device only needs to handle speaker type interface
 *
 */
#define TOTAL_HANDLER 1

/**
 * @brief index of the pointer to the speaker handler
 *
 */
#define SPKR_HANDLER_INDEX 0

/**
 * @brief initialize list of handler
 *
 */
#define INITIALIZE_HANDLER()                \
  static SpeakerCtrlHandler spkrController; \
  static ParentHandler*     _handlerList[TOTAL_HANDLER] = {&spkrController};

#endif