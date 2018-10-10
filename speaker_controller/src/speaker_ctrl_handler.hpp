#ifndef _SPEAKER_CONTROL_HPP
#define _SPEAKER_CONTROL_HPP

#include "parent_handler.hpp"

#include "event_handler_util.hpp"

#include "frozen.h"
#include "mgos.h"
#include "mgos_mqtt.h"

class SpeakerCtrlHandler : public ParentHandler {
 public:
  HandlerError handleRequest(struct mg_connection* mgCon,
                             struct mg_str*        message,
                             char*                 commandName,
                             char*                 response);
  HandlerError handleReport(char* stateReport);
  ~SpeakerCtrlHandler(void){};
  SpeakerCtrlHandler(void);

  static void pwrInputIntHandler(int pin, void* arg);

  void checkPotVoltage(void);

 private:
  void updatePotAdcVal(void);

  HandlerError switchPwr(const bool& isPowerOn);
  HandlerError setVolume(const int32_t& targetVolume);
  HandlerError setMute(const bool& isMuted);
  HandlerError changeWiperDir(const bool& isUp);
  bool         switchIsOn_;
  int32_t      currentVolume_;
  bool         isMuted_;
  int32_t      lastKnownPotAdcVal_;
};

#endif