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
 * @brief topic that the pc controller subscribe to
 */
#define SUBSCRIBE_TOPIC "/pcSpkrReq"
/**
 * @brief topic that the pc controller will publish replise to
 */
#define PUBLISH_TOPIC "/pcSpkrRes"

/****************Pinout Config**************************/

/**
 * @brief pins used to control the relay controlling the pc motherboard pins
 */
#define SPKR_INC_PIN 5  // D1
#define SPKR_DIR_PIN 6  // TODO: change these things
#define SPKR_SELECT_PIN 7

#define VOLUME_UP_LIMIT 100
#define VOLUME_DOWN_LIMIT 0

// TODO: maybe support external button too
/**
 * @brief pins used to sense the pc case switch actuation
 */
#define PC_BUTTON_PIN 4  // D2

/**
 * @brief analog pins used for reading temperature sensor reading
 */
#define ANALOG_PIN 0

#define SPKR_VOLUME_DELAY_MS 1
#define SPKR_INC_ACTIVATED_STATE 0  // active low pin
#define SPKR_PIN_UP_STATE 1         // TODO: calibrate
#define SPKR_SELECT_ACTIVATED 0

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
#define REGISTER_INTERRUPT()                                               \
  do {                                                                     \
    mgos_gpio_set_button_handler(PC_BUTTON_PIN,                            \
                                 MGOS_GPIO_PULL_UP,                        \
                                 MGOS_GPIO_INT_EDGE_NEG,                   \
                                 BUTTON_DEBOUNCE_MS,                       \
                                 PowerCtrlHandler::buttonInterruptHandler, \
                                 NULL);                                    \
  } while (0)

/**
 * @brief change this function to customize the list of handler
 *
 */
#define INITIALIZE_HANDLER()                \
  static SpeakerCtrlHandler spkrController; \
  static ParentHandler*     _handlerList[TOTAL_HANDLER] = {&spkrController};

#endif