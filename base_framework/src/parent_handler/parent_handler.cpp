/**
 * @brief contain the source files for the ParentHandler class
 *
 * @file parent_handler.cpp
 * @author Khoi Trinh
 * @date 2018-09-01
 */

#include "parent_handler.hpp"

#include <stdarg.h>
#include <cstring>

#include "event_handler_util.hpp"

#include "frozen.h"
#include "mgos.h"
#include "mgos_mqtt.h"

/**
 * @brief length in characters of the temporary buffer used for holding a temp json string before it
 * can be copied to the string returned to the master handler
 *
 */
#define TEMP_BUF_LEN 700

/**
 * @brief used for generating a json report string based on the format given, usually child handler
 * uses this to prepare replies to the aws lambda server
 *
 * @param report the string to write the report to
 * @param fmt format of the report, can be found in aws_iot_fmt.hpp
 * @param totalArg total arguments to be printed for the report string
 * @param ...
 * @return HandlerError errors that functions encountered
 */
HandlerError ParentHandler::createReport(char* report, const char* fmt, uint8_t totalArg, ...) {
  va_list vaList;
  va_start(vaList, totalArg);
  HandlerError    errCode = HANDLER_NO_ERR;
  uint32_t        totalByte;
  char            tempBuf[TEMP_BUF_LEN] = "";
  struct json_out jsonBuf               = JSON_OUT_BUF(tempBuf, TEMP_BUF_LEN);

  if ((totalByte = json_vprintf(&jsonBuf, fmt, vaList)) <= 0) {
    errCode = JSON_VPRINTF_ERROR;
  } else {
    strcat(report, jsonBuf.u.buf.buf);
  }
  LOG(LL_INFO, ("Total bytes printed out: %d", totalByte));
  va_end(vaList);
  return errCode;
}

/**
 * @brief used to check if a handler object can handle a request with a specific namespace
 *
 * @param requestNameSpace the namespace extracted from the message
 * @return true if can handle the request
 * @return false if can not handle the request
 *
 * compare the namespace given with the namespace that the object says it supports and return
 * result
 */
bool ParentHandler::canHandleRequest(char* requestNameSpace) {
  return ((0 == strcmp(requestNameSpace, ParentHandler::_nameSpace)) ? true : false);
}

/**
 * @brief return the command namespace that the handler support
 *
 * @return char* the namespace that the command support
 */
char* ParentHandler::getNameSpace(void) { return ParentHandler::_nameSpace; }