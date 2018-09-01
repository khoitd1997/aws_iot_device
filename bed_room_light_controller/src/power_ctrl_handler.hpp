#ifndef _POWER_CTRL_HANDLER_HPP
#define _POWER_CTRL_HANDLER_HPP

#include "parent_handler.hpp"

#include "event_handler_util.hpp"

#include "frozen.h"
#include "mgos.h"
#include "mgos_mqtt.h"

class PowerCtrlHandler : public ParentHandler {
 public:
  HandlerError handleRequest(struct mg_connection* mgCon,
                             struct mg_str*        message,
                             char*                 commandName,
                             char*                 response);
  HandlerError handleReport(char* stateReport);
  ~PowerCtrlHandler(void){};
  PowerCtrlHandler(void);
  static void setLightStatus(bool lightStatus);
  static bool getLightStatus(void);

 private:
  static bool _lightIsOn;
};

#endif