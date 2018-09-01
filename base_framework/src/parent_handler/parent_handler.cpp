#include "parent_handler.hpp"

#include <stdarg.h>
#include <cstring>

#include "event_handler_util.hpp"

#include "frozen.h"
#include "mgos.h"
#include "mgos_mqtt.h"

#define TEMP_BUF_LEN 700

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

bool ParentHandler::canHandleRequest(char* requestNameSpace) {
  return ((0 == strcmp(requestNameSpace, ParentHandler::_nameSpace)) ? true : false);
}

char* ParentHandler::getNameSpace(void) { return ParentHandler::_nameSpace; }