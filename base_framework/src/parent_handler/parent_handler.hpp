#ifndef _PARENT_HANDLER_HPP
#define _PARENT_HANDLER_HPP

#include "event_handler_util.hpp"

#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include "frozen.h"
#include "mgos.h"
#include "mgos_mqtt.h"

class ParentHandler {
 public:
  virtual ~ParentHandler(void) {}
  virtual HandlerError handleRequest(struct mg_connection* mgCon,
                                     struct mg_str*        message,
                                     char*                 commandName,
                                     char*                 response)   = 0;
  virtual HandlerError handleReport(char* stateReport) = 0;

  HandlerError createReport(char* report, const char* fmt, uint8_t totalArg, ...);

  bool  canHandleRequest(char* requestNameSpace);
  char* getNameSpace(void);

 protected:
  char _nameSpace[MAX_NAMESPACE_LEN];
};

#endif