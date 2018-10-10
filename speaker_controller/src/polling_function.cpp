#include "master_handler.hpp"

#include "device_config.hpp"
#include "mgos.h"
#include "mgos_mqtt.h"
#include "speaker_ctrl_handler.hpp"

/**
 * @brief poll the voltage on the potentiometer and change the volume if there is a change in
 * voltage
 *
 * @param mgCon object representing connection to the internet
 * @param _handlerList the list of handler that user defined
 * @param jsonBuf if you write something to this buffer, it will be published to the mqtt server of
 * the device
 */
void handlePolling(struct mg_connection *mgCon, ParentHandler *handlerList[], char *jsonBuf) {
  (static_cast<SpeakerCtrlHandler *>(handlerList[SPKR_HANDLER_INDEX]))->checkPotVoltage();
  (void)jsonBuf;
  (void)mgCon;
}
