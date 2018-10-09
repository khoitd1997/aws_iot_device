#include "speaker_ctrl_handler.hpp"

#include <cstring>

#include "aws_iot_fmt.hpp"
#include "device_config.hpp"
#include "event_handler_util.hpp"

#include "frozen.h"
#include "mgos.h"
#include "mgos_adc.h"
#include "mgos_mqtt.h"

SpeakerCtrlHandler::SpeakerCtrlHandler(void) : currentVolume_(0), isMuted_(true) {
  strcpy(ParentHandler::_nameSpace, "Alexa.Speaker");
}

HandlerError SpeakerCtrlHandler::handleRequest(struct mg_connection* mgCon,
                                               struct mg_str*        message,
                                               char*                 commandName,
                                               char*                 response) {
  if ((0 != strcmp(commandName, "SetVolume")) && (0 != strcmp(commandName, "AdjustVolume")) &&
      (0 != strcmp(commandName, "SetMute"))) {
    return MQTT_ERR_UNKNOWN_COMMAND;
  }

  if (0 == strcmp(commandName, "SetVolume")) {
    uint32_t targetVol = 0;

    getAwsPayload(message, SPKR_SET_VOL_PAYLOAD_FMT, SPKR_SET_VOL_PAYLOAD_TOTAL_ARG, &targetVol);
    setVolume(targetVol);

  } else if (0 == strcmp(commandName, "AdjustVolume")) {
    uint32_t adjustVol    = 0;
    bool     isDefaultVol = false;

    getAwsPayload(message,
                  SPKR_ADJUST_VOL_PAYLOAD_FMT,
                  SPKR_ADJUST_VOL_PAYLOAD_TOTAL_ARG,
                  &adjustVol,
                  &isDefaultVol);
    setVolume(currentVolume_ + adjustVol);

  } else if ((0 == strcmp(commandName, "SetMute"))) {
    bool isMuted = false;
    getAwsPayload(message, SPKR_SET_MUTE_PAYLOAD_FMT, SPKR_SET_MUTE_PAYLOAD_TOTAL_ARG, &isMuted);
    setMute(isMuted);
  }

  return createReport(response,
                      SPKR_CTRL_FMT,
                      SPKR_CTRL_TOTAL_ARG,
                      "name",
                      "volume",
                      "namespace",
                      _nameSpace,
                      "value",
                      currentVolume_,
                      "name",
                      "muted",
                      "namespace",
                      _nameSpace,
                      "value",
                      isMuted_);

  (void)mgCon;
}

HandlerError SpeakerCtrlHandler::handleReport(char* stateReport) {
  return createReport(stateReport,
                      SPKR_CTRL_FMT,
                      SPKR_CTRL_TOTAL_ARG,
                      "name",
                      "volume",
                      "namespace",
                      _nameSpace,
                      "value",
                      currentVolume_,
                      "name",
                      "muted",
                      "namespace",
                      _nameSpace,
                      "value",
                      isMuted_);
}

HandlerError SpeakerCtrlHandler::setVolume(const int32_t& targetVolume) {
  const int32_t targetVol =
      targetVolume >= VOLUME_UP_LIMIT
          ? VOLUME_UP_LIMIT
          : ((targetVolume <= VOLUME_DOWN_LIMIT) ? VOLUME_DOWN_LIMIT : targetVolume);

  if (targetVol == currentVolume_) { return HANDLER_NO_ERR; }

  // activated the chip
  write_pin(SPKR_SELECT_PIN, SPKR_SELECT_ACTIVATED);

  // unmute device automatically when user set volume
  setMute(false);

  // change adjustment direction
  (currentVolume_ < targetVol) ? changeWiperDir(true) : changeWiperDir(false);

  // start toggling the pin
  for (int32_t volumeIndex = 0; volumeIndex < abs(currentVolume_ - targetVol); ++volumeIndex) {
    write_pin(SPKR_INC_PIN, SPKR_INC_ACTIVATED_STATE);
    mgos_msleep(SPKR_VOLUME_DELAY_MS);  // delay to make sure the switch has effect
    write_pin(SPKR_INC_PIN, !SPKR_INC_ACTIVATED_STATE);
    mgos_msleep(SPKR_VOLUME_DELAY_MS);
  }

  currentVolume_ = targetVol;

  return HANDLER_NO_ERR;
}

HandlerError SpeakerCtrlHandler::setMute(const bool& isMuted) {
  if (isMuted) {
    // place the chip in power down mode
    write_pin(SPKR_INC_PIN, !SPKR_INC_ACTIVATED_STATE);
    write_pin(SPKR_SELECT_PIN, !SPKR_SELECT_ACTIVATED);
  } else {
    // activate the chip
    write_pin(SPKR_SELECT_PIN, SPKR_SELECT_ACTIVATED);
  }
  isMuted_ = isMuted;
  return HANDLER_NO_ERR;
}

HandlerError SpeakerCtrlHandler::changeWiperDir(const bool& isUp) {
  return write_pin(SPKR_DIR_PIN, (isUp) ? SPKR_PIN_UP_STATE : !SPKR_PIN_UP_STATE);
}