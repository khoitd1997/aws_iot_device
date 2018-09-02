/**
 * @brief
 *
 * @file temp_sensor_handler.hpp
 * @author Khoi Trinh
 * @date 2018-09-01
 */

#ifndef _TEMP_SENSOR_HANDLER_HPP
#define _TEMP_SENSOR_HANDLER_HPP

#include "parent_handler.hpp"

#include "event_handler_util.hpp"

#include "frozen.h"
#include "mgos.h"
#include "mgos_mqtt.h"

/**
 * @brief
 *
 */
class TempSensorHandler : public ParentHandler {
 public:
  TempSensorHandler(void);
  HandlerError handleRequest(struct mg_connection* mgCon,
                             struct mg_str*        message,
                             char*                 commandName,
                             char*                 response);
  HandlerError handleReport(char* stateReport);

  ~TempSensorHandler(void) {}

 private:
};

#endif