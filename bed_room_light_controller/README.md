# Bed room light controller

This is the source code for the bed room light controller, the idea is to use an mcu like the esp8266/32 to turn on/off the light based on Alexa command

## Circuitry

The light controller would use one pin, which would then trigger a relay that is controlling the AC continuity of the main light

## AWS IoT API

The light controller implements only the power controller interface of the aws iot API