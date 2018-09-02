/**
 * @brief TempSensorHandler source files for pc controller
 *
 * @file temp_sensor_handler.cpp
 * @author Khoi Trinh
 * @date 2018-09-01
 */

#include "temp_sensor_handler.hpp"

#include <cstring>

#include "aws_iot_fmt.hpp"
#include "device_config.hpp"
#include "event_handler_util.hpp"

#include "frozen.h"
#include "mgos.h"
#include "mgos_adc.h"
#include "mgos_mqtt.h"

/**
 * @brief TMP36 coefficient to convert from analog voltage to celsius
 *
 */
#define TMP36_COEFF 3300 / 1024

TempSensorHandler::TempSensorHandler() { strcpy(_nameSpace, "Alexa.TemperatureSensor"); }

/**
 * @brief handle aws request pertainting to temperature sensor endpoint for the pc controller
 * The function will mesasure the temperature(using the TMP36 sensor for now) and put it in areport
 * and write it to a buffer
 *
 * @param mgCon struct representing connection to server
 * @param message message sent to the mcu by the server
 * @param response buffer to write the response to
 * @return HandlerError
 */
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

/**
 * @brief handle state report pertainting to temperature sensor endpoint for the pc controller
 *
 * @param stateReport buffer to write report to
 * @return HandlerError
 */
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