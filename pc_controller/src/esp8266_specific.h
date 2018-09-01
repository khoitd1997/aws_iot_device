#ifndef _ESP8266_SPECIFIC_H
#define _ESP8266_SPECIFIC_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define PC_CTRL_PIN 5    // D1
#define PC_BUTTON_PIN 4  // D2

// source: https://forum.mongoose-os.com/discussion/1218/unable-to-read-adc-value
#define ANALOG_PIN 17

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif