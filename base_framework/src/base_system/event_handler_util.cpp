#include "event_handler_util.hpp"

#include "mgos.h"
#include "mgos_mqtt.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

// https://forum.mongoose-os.com/discussion/2290/frozen-json-scanf-quotation-marks (json scanf
// question)

HandlerError create_error_message(const HandlerError error,
                                  struct mg_str*     payload,
                                  char*              errorMessage) {
  static ErrDescript errDescript[] = {{MQTT_ERR_UNKNOWN_COMMAND, "mqtt received unknown command"},
                                      {MQTT_ERR_GPIO, "GPIO error"},
                                      {JSON_NOT_ENOUGH_ARG, "not enough arg for json"},
                                      {MQTT_CONFIG_NOT_SET, "mqtt config not set"},
                                      {JSON_UNKNOWN_NAMESPACE, "unknown namespace"},
                                      {JSON_VPRINTF_ERROR, "problem with json vprintf"},
                                      {HANDLER_NULL, "null handler detected"}};

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

HandlerError write_pin(const uint8_t pinNum, const uint8_t pinState) {
  LOG(LL_INFO, ("Writing to pins %d with state %d", pinNum, pinState));
  if (false == mgos_gpio_set_mode(pinNum, MGOS_GPIO_MODE_OUTPUT)) { return MQTT_ERR_GPIO; }
  mgos_gpio_write(pinNum, (pinState > 0 ? true : false));
  return HANDLER_NO_ERR;
}

HandlerError read_pin(const uint8_t pinNum, uint8_t* pinState) {
  LOG(LL_INFO, ("Reading Pins %d", pinNum));
  *pinState = (mgos_gpio_read(pinNum) ? 1 : 0);
  return HANDLER_NO_ERR;
}

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

HandlerError printStateReportOpen(char* jsonOut) {
  strcat(jsonOut, "{\"properties\": [");
  return HANDLER_NO_ERR;
}

HandlerError printStateReportDelimiter(char* jsonOut) {
  strcat(jsonOut, ", ");
  return HANDLER_NO_ERR;
}

HandlerError printStateReportEnd(char* jsonOut) {
  strcat(jsonOut, "]}");
  return HANDLER_NO_ERR;
}