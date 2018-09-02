# PC Controller

The pc controller is an iot device intended to automate task related to desktop pc like turning them on/off, or monitoring their specs(like temperature inside the chassis)

## Circuitry

The circuitry is really simple, the mcu controls a relay that would short the PowerOn pins of the motherboard, it also utilize an analog pins to read temperature from an analog temperature sensor

## AWS IoT API

The pc controller implements the power controller API and the temperature sensor API