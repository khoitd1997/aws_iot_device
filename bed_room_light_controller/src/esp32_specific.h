#ifndef _ESP32_SPECIFIC_H
#define _ESP32_SPECIFIC_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define LIGHT_CTRL_PIN 21  // marked P21 on the board

// source: https://forum.mongoose-os.com/discussion/1218/unable-to-read-adc-value
#define ANALOG_PIN 17

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif