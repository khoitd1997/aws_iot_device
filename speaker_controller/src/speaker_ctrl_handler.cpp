/** @defgroup speaker_controller Speaker Controller */

/** @addtogroup speaker_controller
 *
 * @{
 */

/**
 * @file speaker_ctrl_handler.cpp
 * @author Khoi Trinh
 * @brief implemntation file for speaker handler
 * @version 0.1
 * @date 2018-10-10
 *
 *
 */

#include "speaker_ctrl_handler.hpp"

#include <cstring>

#include "aws_iot_fmt.hpp"
#include "device_config.hpp"
#include "event_handler_util.hpp"

#include "frozen.h"
#include "mgos.h"
#include "mgos_adc.h"
#include "mgos_gpio.h"
#include "mgos_mqtt.h"

/**
 * @brief handler function for when the user turn on/off the physical pot on the speaker
 * This function will either turn on/off the speaker based on the user interaction with the
 * physical potentiometer
 * @param pin the pin that triggered the handler
 * @param arg pointer to things passed by mongoose OS to the handler, in this case, the pointer to
 * the speaker handler is registered to be the argument passed
 */
void SpeakerCtrlHandler::pwrInputIntHandler(int pin, void* arg) {
  LOG(LL_INFO, ("Spkr button interrupt: %d", pin));

  SpeakerCtrlHandler* speakerCtrl = static_cast<SpeakerCtrlHandler*>(arg);
  if (SPKR_PWR_INPUT_PIN == pin) {
    uint8_t pinState = 0;
    mgos_msleep(30);  // make sure the pin state finishes transition
    read_pin(SPKR_PWR_INPUT_PIN, &pinState);
    speakerCtrl->switchIsOn_ = (SPKR_PWR_INPUT_OFF_STATE == pinState) ? false : true;
    speakerCtrl->setMute(!(speakerCtrl->switchIsOn_));
    LOG(LL_INFO, ("handling power: %d", pinState));
  }
  (void)pin;
  (void)arg;
}

/**
 * @brief Construct a new Speaker Ctrl Handler:: Speaker Ctrl Handler object
 * create a new spekaer handler, intialize the namespace argument, prepare analog, and interrupt
 * pins, set speaker to mute
 */
SpeakerCtrlHandler::SpeakerCtrlHandler(void)
    : switchIsOn_(true), currentVolume_(0), isMuted_(true), lastKnownPotAdcVal_(0) {
  mgos_adc_enable(SPKR_VOL_LEVEL_INPUT_PIN);
  updatePotAdcVal();
  strcpy(ParentHandler::_nameSpace, "Alexa.Speaker");

  // interupt register here to in order to access this object data
  mgos_gpio_set_mode(SPKR_PWR_INPUT_PIN, MGOS_GPIO_MODE_INPUT);
  mgos_gpio_set_pull(SPKR_PWR_INPUT_PIN, MGOS_GPIO_PULL_UP);
  mgos_gpio_set_int_handler(
      SPKR_PWR_INPUT_PIN, MGOS_GPIO_INT_EDGE_ANY, SpeakerCtrlHandler::pwrInputIntHandler, this);
  mgos_gpio_enable_int(SPKR_PWR_INPUT_PIN);

  setMute(isMuted_);
}

/**
 * @brief function to handle request passed by aws mqtt
 * Depending on the reques type, the handler will either mute the device, adjust volume or set new
 * volume entirely
 *
 * @param mgCon mongoose OS connection to the internet
 * @param message message received by mongoose OS
 * @param commandName the command extracted from the message
 * @param response the buffer to write the response to, which will be published to the mqtt response
 * server
 * @return HandlerError return errors reported by function inside the handler
 */
HandlerError SpeakerCtrlHandler::handleRequest(struct mg_connection* mgCon,
                                               struct mg_str*        message,
                                               char*                 commandName,
                                               char*                 response) {
  HandlerError errorCode = HANDLER_NO_ERR;

  LOG(LL_INFO, ("Handling Speaker Event"));

  mgos_gpio_disable_int(SPKR_PWR_INPUT_PIN);

  if ((0 != strcmp(commandName, "SetVolume")) && (0 != strcmp(commandName, "AdjustVolume")) &&
      (0 != strcmp(commandName, "SetMute"))) {
    return MQTT_ERR_UNKNOWN_COMMAND;
  }

  if (0 == strcmp(commandName, "SetVolume")) {
    uint32_t targetVol = 0;

    if (HANDLER_NO_ERR !=
        (errorCode = getAwsPayload(
             message, SPKR_SET_VOL_PAYLOAD_FMT, SPKR_SET_VOL_PAYLOAD_TOTAL_ARG, &targetVol))) {
      return errorCode;
    }

    setVolume(targetVol);

  } else if (0 == strcmp(commandName, "AdjustVolume")) {
    LOG(LL_INFO, ("Adjusting Volume"));
    uint32_t adjustVol    = 0;
    int32_t  isDefaultVol = 0;

    if (HANDLER_NO_ERR != (errorCode = getAwsPayload(message,
                                                     SPKR_ADJUST_VOL_PAYLOAD_FMT,
                                                     SPKR_ADJUST_VOL_PAYLOAD_TOTAL_ARG,
                                                     &adjustVol,
                                                     &isDefaultVol))) {
      return errorCode;
    }

    setVolume(currentVolume_ + adjustVol);

  } else if ((0 == strcmp(commandName, "SetMute"))) {
    int32_t isMuted = 0;
    LOG(LL_INFO, ("Handling Set Mute"));

    if (HANDLER_NO_ERR !=
        (errorCode = getAwsPayload(
             message, SPKR_SET_MUTE_PAYLOAD_FMT, SPKR_SET_MUTE_PAYLOAD_TOTAL_ARG, &isMuted))) {
      return errorCode;
    }
    setMute((bool)isMuted);
  }

  mgos_gpio_enable_int(SPKR_PWR_INPUT_PIN);

  LOG(LL_INFO, ("Handling Speaker Event, muted: %d", isMuted_));
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

/**
 * @brief handle preparing report for the speaker handler
 * used when the handler needs to respond to the , will report whether the speaker is muted and what
 * the volume is
 *
 * @param stateReport the buffer to write the report to
 * @return HandlerError return problem error
 */
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

/**
 * @brief set the volume on the speaker by adjusting the digital pot
 *
 * Depending on whether the volume is higher/lower than the current volume, the wiper direction is
 * adjusted and moved accordingly
 *
 * @param targetVolume the volume to reach
 * @return HandlerError return problem error
 */
HandlerError SpeakerCtrlHandler::setVolume(const int32_t& targetVolume) {
  const int32_t targetVol =
      targetVolume >= VOLUME_UP_LIMIT
          ? VOLUME_UP_LIMIT
          : ((targetVolume <= VOLUME_DOWN_LIMIT) ? VOLUME_DOWN_LIMIT : targetVolume);

  if (targetVol == currentVolume_) { return HANDLER_NO_ERR; }

  // unmute device automatically when user set volume
  setMute(false);

  // change adjustment direction
  (currentVolume_ < targetVol) ? changeWiperDir(true) : changeWiperDir(false);
  mgos_msleep(1);

  // activated the chip
  write_pin(SPKR_SELECT_PIN, SPKR_SELECT_ACTIVATED);
  mgos_msleep(1);

  // start toggling the pin
  for (int32_t volumeIndex = 0; volumeIndex < abs(currentVolume_ - targetVol); ++volumeIndex) {
    write_pin(SPKR_INC_PIN, SPKR_INC_ACTIVATED_STATE);
    mgos_msleep(SPKR_VOLUME_DELAY_MS);  // delay to make sure the switch has effect
    write_pin(SPKR_INC_PIN, !SPKR_INC_ACTIVATED_STATE);
    mgos_msleep(SPKR_VOLUME_DELAY_MS);
  }

  // record current adc level so that polling doesn't change the newly set volume
  updatePotAdcVal();
  currentVolume_ = targetVol;

  return HANDLER_NO_ERR;
}

/**
 * @brief used for mute/unmuting the speaker
 * mute the speaker by turning off the power, also update pot value to prevent noise from causing
 * immature volume change
 *
 * @param isMuted if true, mute the speaker
 * @return HandlerError return problem error
 */
HandlerError SpeakerCtrlHandler::setMute(const bool& isMuted) {
  (isMuted ? switchPwr(false) : switchPwr(true));
  isMuted_ = isMuted;
  updatePotAdcVal();
  return HANDLER_NO_ERR;
}

/**
 * @brief change the wiper direction of the digital pot
 * interact with the DIR pin of the digital pot to change the direction of the wiper
 *
 * @param isUp if true, change the direction to moving up the resistance
 * @return HandlerError return problem error
 */
HandlerError SpeakerCtrlHandler::changeWiperDir(const bool& isUp) {
  return write_pin(SPKR_DIR_PIN, (isUp) ? SPKR_PIN_UP_STATE : !SPKR_PIN_UP_STATE);
}

/**
 * @brief used for switching on/off the power to the speaker
 * mainly serve as a utility function used by other functions to switch on/off speaker
 *
 * @param isPowerOn turn speaker on if true
 * @return HandlerError return error problems
 */
HandlerError SpeakerCtrlHandler::switchPwr(const bool& isPowerOn) {
  (isPowerOn ? write_pin(SPKR_PWR_PIN, SPKR_PWR_ON_STATE)
             : write_pin(SPKR_PWR_PIN, !SPKR_PWR_ON_STATE));
  return HANDLER_NO_ERR;
}

/**
 * @brief called by the main polling function to check and update the pot voltage if necessary
 * read the adc voltage and if the voltage changes over a certain threshold, which indicates that
 * the user has moved the pot, then the volume will be adjusted accordingly
 */
void SpeakerCtrlHandler::checkPotVoltage(void) {
  int32_t adcReading = mgos_adc_read(SPKR_VOL_LEVEL_INPUT_PIN);
  LOG(LL_INFO, ("Adc reading: %d", adcReading));
  if (adcReading < 0) {
    return;  // should be just noise
  }
  if (abs(lastKnownPotAdcVal_ - adcReading) >= SPKR_VOL_LEVEL_INPUT_THRESHOLD) {
    lastKnownPotAdcVal_ = adcReading;
    if (switchIsOn_) { setVolume((adcReading * VOLUME_UP_LIMIT) / SPKR_VOL_LEVEL_MAX_COUNT); }
  }
}

/**
 * @brief update the value read from the user input potentiometer
 * Will wait some time to make sure the adc value settle before reading and updating the last known
 * pot value
 */
void SpeakerCtrlHandler::updatePotAdcVal(void) {
  mgos_msleep(20);  // wait for noise to stabilize
  lastKnownPotAdcVal_ = mgos_adc_read(SPKR_VOL_LEVEL_INPUT_PIN);
}

/** @} */
