/** @addtogroup bed_room_light_controller 
* 
* @{ 
*/ 

/**
 * @file polling_function.cpp
 * @author Khoi Trinh
 * @brief dummy file carrying empty polling function since bed room light controller has no polling
 * need
 * @version 0.1
 * @date 2018-10-10
 *
 */

#include "master_handler.hpp"

#include "device_config.hpp"
#include "mgos.h"
#include "mgos_mqtt.h"

/**
 * @brief dummy empty function since bed room light controller has no polling
 * need
 *
 * @param mgCon mongoose network connection
 * @param handlerList list of pointer to handler object
 * @param jsonBuf json buffer for writing data
 */
void handlePolling(struct mg_connection *mgCon, ParentHandler *handlerList[], char *jsonBuf) {
  (void)handlerList;
  (void)jsonBuf;
  (void)mgCon;
}

/** @} */
