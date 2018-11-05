/** @addtogroup base_framework 
* 
* @{ 
*/ 

/**
 * @brief folder carrying utils functions that are helpful either to the master handler or to the
 * child handler
 *
 * The folder implements important things like error message creation, aws state report printing
 * helper function
 *
 * @file event_handler_util.cpp
 * @author Khoi Trinh
 * @date 2018-09-01
 */

#include "event_handler_util.hpp"

#include "mgos.h"
#include "mgos_mqtt.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

/**
 * @brief create error message based on the error code given and stores it in the given buffer
 * @param error error code of the problems
 * @param payload the payload of the message received by the mcu
 * @param errorMessage the buffer to store the error message
 * @return return any errors that are encountered
 *
 * The function stores a list of struct that stores the error code and their corresponding error
 * message, the function searches through the struct to find the correct message
 */
HandlerError create_error_message(const HandlerError error,
                                  struct mg_str*     payload,
                                  char*              errorMessage) {
  static ErrDescript errDescript[] = {{MQTT_ERR_UNKNOWN_COMMAND, "mqtt received unknown command"},
                                      {MQTT_ERR_GPIO, "GPIO error"},
                                      {JSON_NOT_ENOUGH_ARG, "not enough arg for json"},
                                      {MQTT_CONFIG_NOT_SET, "mqtt config not set"},
                                      {JSON_UNKNOWN_NAMESPACE, "unknown namespace"},
                                      {JSON_VPRINTF_ERROR, "problem with json vprintf"},
                                      {HANDLER_NULL, "null handler detected"},
                                      {JSON_VSCANF_ERROR, "problem with json vscanf"}};

  // finding matching error message for error code
  uint8_t errorDesc;
  for (errorDesc = 0; errorDesc < (sizeof(errDescript) / sizeof(errDescript[0])); ++errorDesc) {
    if (error == (errDescript[errorDesc]).errorCode) {
      strcat(errorMessage, "{");
      strcat(errorMessage, "\"");
      strcat(errorMessage, "error\": \"");
      strcat(errorMessage, (errDescript[errorDesc]).errorMessage);
      strcat(errorMessage, "\"");
      strcat(errorMessage, "}");
      break;
    }
  }

  if (errorDesc == (sizeof(errDescript) / sizeof(errDescript[0]))) {
    strcpy(errorMessage, "Unknown Error");
  }

  LOG(LL_ERROR, ("%s, payload: %s", errorMessage, payload->p));

  return HANDLER_NO_ERR;
}

/**
 * @brief used for writing to a digital pin given its pin number and state
 *
 * @param pinNum pin to write
 * @param pinState state of the pin, 0 for off, 1 for on
 * @return HandlerError return error code
 */
HandlerError write_pin(const uint8_t pinNum, const uint8_t pinState) {
  // LOG(LL_INFO, ("Writing to pins %d with state %d", pinNum, pinState));
  if (false == mgos_gpio_set_mode(pinNum, MGOS_GPIO_MODE_OUTPUT)) { return MQTT_ERR_GPIO; }
  mgos_gpio_write(pinNum, (pinState > 0 ? true : false));
  return HANDLER_NO_ERR;
}

/**
 * @brief used to reat the status of a digital pin
 *
 * @param pinNum pin number to read
 * @param pinState pointer to write the result of the read to
 * @return Handler Error problem error code
 */
HandlerError read_pin(const uint8_t pinNum, uint8_t* pinState) {
  // LOG(LL_INFO, ("Reading Pins %d", pinNum));
  *pinState = (mgos_gpio_read(pinNum) ? 1 : 0);
  return HANDLER_NO_ERR;
}

/**
 * @brief get command name as well as command namespace of messages sent from aws lambda
 *
 * @param message message received by the mcu through mqtt
 * @param commandName name of the command like "TurnOff"
 * @param nameSpace namespace of the command like "Alexa.PowerController"
 * @return HandlerError error code
 */
HandlerError getCommandInfo(struct mg_str* message, char* commandName, char* nameSpace) {
  assert(message);

  char* tempName      = NULL;
  char* tempNameSpace = NULL;

  if (2 ==
      json_scanf(
          message->p, message->len, "{name: %Q, namespace: %Q", &(tempName), &(tempNameSpace))) {
    strcpy(commandName, tempName);
    strcpy(nameSpace, tempNameSpace);

    // the jscon_scanf function uses malloc so we must free it
    free(tempName);
    free(tempNameSpace);
    return HANDLER_NO_ERR;
  } else {
    return JSON_NOT_ENOUGH_ARG;
  }
}

/**
 * @brief Get the Aws Payload json key
 *
 * @param message messsage received by mongoose OS from mqtt server
 * @param awsPayloadFormat the payload format of the specific aws device type such as speaker, power
 * controller, etc
 * @param totalArg how many arg to parse in the payload format
 * @return HandlerError return error code in case that there is not enough elements to parse
 */
HandlerError getAwsPayload(const struct mg_str* message,
                           const char*          awsPayloadFormat,
                           const uint8_t&       totalArg,
                           ...) {
  va_list vaList;
  va_start(vaList, totalArg);
  HandlerError errCode      = HANDLER_NO_ERR;
  uint32_t     totalElement = 0;

  if (totalArg !=
      (totalElement = json_vscanf(message->p, message->len, awsPayloadFormat, vaList))) {
    errCode = JSON_VSCANF_ERROR;
  }
  LOG(LL_INFO, ("Total Payload Scanned: %d", totalElement));
  va_end(vaList);
  return errCode;
}

/**
 * @brief print the beginning of an aws smarthome state report
 *
 * @param jsonOut the buffer to store the report
 * @return HandlerError problem error code
 */
HandlerError printStateReportOpen(char* jsonOut) {
  strcat(jsonOut, "{\"properties\": [");
  return HANDLER_NO_ERR;
}

/**
 * @brief print the delimiter between endpoints reports in aws iot state report
 *
 * @param jsonOut buffer to store the json report
 * @return HandlerError problem error code
 */
HandlerError printStateReportDelimiter(char* jsonOut) {
  strcat(jsonOut, ", ");
  return HANDLER_NO_ERR;
}

/**
 * @brief print the ending part of an aws iot state report
 *
 * @param jsonOut buffer to store the json report
 * @return HandlerError problem error code
 */
HandlerError printStateReportEnd(char* jsonOut) {
  strcat(jsonOut, "]}");
  return HANDLER_NO_ERR;
}

/** @} */
