#include "temp_sensor_handler.hpp"

#include <cstring>

#include "esp8266_specific.h"
#include "event_handler_util.hpp"

#include "frozen.h"
#include "mgos.h"
#include "mgos_adc.h"
#include "mgos_mqtt.h"

#define TEMP_SENSOR_FMT "{%Q: %Q, %Q: %Q, %Q: {%Q: %.1f, %Q: %Q}}"
#define TEMP_SENSOR_TOTAL_ARG 9

#define TMP36_COEFF 3300 / 1024

TempSensorHandler::TempSensorHandler() { strcpy(_nameSpace, "Alexa.TemperatureSensor"); }

HandlerError TempSensorHandler::handleRequest(struct mg_connection* mgCon,
                                              struct mg_str*        message,
                                              char*                 commandName,
                                              char*                 response) {
  (void)message;  // satisfy warning
  (void)mgCon;
  (void)response;
  (void)commandName;
  return HANDLER_NO_ERR;
}
HandlerError TempSensorHandler::handleReport(char* stateReport) {
  mgos_adc_enable(ANALOG_PIN);
  float volt = (float)mgos_adc_read(ANALOG_PIN);

  createReport(stateReport,
               TEMP_SENSOR_FMT,
               TEMP_SENSOR_TOTAL_ARG,
               "name",
               "temperature",
               "namespace",
               _nameSpace,
               "value",
               "value",
               volt,
               "scale",
               "CELSIUS");

  // LOG(LL_INFO, ("The temperature is %f", volt));
  LOG(LL_INFO, ("The temp sensor buf is %s", stateReport));

  return HANDLER_NO_ERR;
}