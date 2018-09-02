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
#include "power_ctrl_handler.hpp"
#include "temp_sensor_handler.hpp"

/****************MQTT Config****************************/
/**
 * @brief topic that the pc controller subscribe to
 */
#define SUBSCRIBE_TOPIC "/pcReq"
/**
 * @brief topic that the pc controller will publish replise to
 */
#define PUBLISH_TOPIC "/pcRes"

/****************Pinout Config**************************/

/**
 * @brief pins used to control the relay controlling the pc motherboard pins
 */
#define PC_CTRL_PIN 5  // D1

/**
 * @brief pins used to sense the pc case switch actuation
 */
#define PC_BUTTON_PIN 4  // D2

/**
 * @brief analog pins used for reading temperature sensor reading
 */
#define ANALOG_PIN 0

/****************Device Handler Config*****************/

/**
 * @brief change this if you use more or less handler
 *
 */
#define TOTAL_HANDLER 2

/**
 * @brief Reigiter pc controller interrupt
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
#define INITIALIZE_HANDLER()              \
  static PowerCtrlHandler  pwrController; \
  static TempSensorHandler tempHandler;   \
  static ParentHandler*    _handlerList[TOTAL_HANDLER] = {&pwrController, &tempHandler};

#endif