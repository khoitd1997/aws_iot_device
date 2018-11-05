/** @addtogroup speaker_controller 
* 
* @{ 
*/ 

/**
 * @file speaker_ctrl_handler.hpp
 * @author Khoi Trinh
 * @brief header file for speaker handler
 * @version 0.1
 * @date 2018-10-10
 *
 */

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

/** @} */
