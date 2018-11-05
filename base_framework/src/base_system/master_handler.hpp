/** @addtogroup base_framework 
* 
* @{ 
*/ 

/**
 * @brief header file for master handler class
 *
 * @file master_handler.hpp
 * @author Khoi Trinh
 * @date 2018-09-01
 */

#ifndef _MASTER_HANDLER_HPP
#define _MASTER_HANDLER_HPP

#include "event_handler_util.hpp"
#include "parent_handler.hpp"

#include "frozen.h"
#include "mgos.h"
#include "mgos_mqtt.h"

/**
 * @brief Maximum characters that the payload received by the mqtt will have
 */
#define MAX_PAYLOAD_SIZE 150
/**
 * @brief Maximum length in character of the response
 */
#define MAX_RESPONSE_LEN 1500

void handleAwsRequest(struct mg_connection *mgCon,
                      struct mg_str *       payload,
                      char *                jsonBuf,
                      ParentHandler *       _handlerList[]);
void handleAllReq(struct mg_connection *mgCon, int eventType, void *rawMessage, void *user_data);

void handlePolling(struct mg_connection *mgCon, ParentHandler *handlerList[], char *jsonBuf);

#endif

/** @} */
