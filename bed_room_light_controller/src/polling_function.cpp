#include "master_handler.hpp"

#include "device_config.hpp"
#include "mgos.h"
#include "mgos_mqtt.h"

void handlePolling(struct mg_connection *mgCon, ParentHandler *handlerList[], char *jsonBuf) {
  (void)handlerList;
  (void)jsonBuf;
  (void)mgCon;
}