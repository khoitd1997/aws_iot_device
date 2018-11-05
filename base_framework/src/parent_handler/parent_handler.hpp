/** @addtogroup base_framework 
* 
* @{ 
*/ 

/**
 * @brief contain class declaration for the ParentHandler class
 *
 * @file parent_handler.hpp
 * @author Khoi Trinh
 * @date 2018-09-01
 */

#ifndef _PARENT_HANDLER_HPP
#define _PARENT_HANDLER_HPP

#include "event_handler_util.hpp"

#include <stdarg.h>
#include <stdint.h>
#include <string.h>

#include "frozen.h"
#include "mgos.h"
#include "mgos_mqtt.h"

/**
 * @brief the parent handler class that is the blueprint for other child handlers to implement, it
 * also defines a couple functions that child handler can use
 */
class ParentHandler {
 public:
  ParentHandler(void);
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
  /**
   * @brief namespace that the handler supports, all child classes must initialize this variable to
   * let the master handler know which namespace it supports
   */
  char _nameSpace[MAX_NAMESPACE_LEN];
};

#endif

/** @} */
