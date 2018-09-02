/**
 * @brief header file for event handler utils
 *
 * @file event_handler_util.hpp
 * @author Khoi Trinh
 * @date 2018-09-01
 */

#ifndef _EVENT_HANDLER_UTIL_H
#define _EVENT_HANDLER_UTIL_H

#include "mgos.h"
#include "mgos_mqtt.h"

#include <stdint.h>

/**
 * @brief maximum length in characters of command name sent by the server
 */
#define MAX_COMMAND_LEN 30

/**
 * @brief maximum length in characters of the namespace name sent by the server
 */
#define MAX_NAMESPACE_LEN 30

/**
 * @brief enum defining errors that the system(including master handler and other child handler) may
 * have
 */
typedef enum {
  HANDLER_NO_ERR = 0,
  MQTT_ERR_UNKNOWN_COMMAND,
  MQTT_ERR_GPIO,
  JSON_NOT_ENOUGH_ARG,
  MQTT_CONFIG_NOT_SET,
  JSON_UNKNOWN_NAMESPACE,
  JSON_VPRINTF_ERROR,
  HANDLER_NULL
} HandlerError;

/**
 * @brief used for storing the error code and its corresponding error message
 */
typedef struct {
  HandlerError errorCode;
  const char*  errorMessage;
} ErrDescript;

HandlerError getCommandInfo(struct mg_str* message, char* commandName, char* nameSpace);

HandlerError write_pin(const uint8_t pinNum, const uint8_t pinState);
HandlerError read_pin(const uint8_t pinNum, uint8_t* pinState);

HandlerError create_error_message(const HandlerError error,
                                  struct mg_str*     payload,
                                  char*              errorMessage);

/* State Report Helper */
HandlerError printStateReportOpen(char* jsonOut);
HandlerError printStateReportDelimiter(char* jsonOut);
HandlerError printStateReportEnd(char* jsonOut);

#endif