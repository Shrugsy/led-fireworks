## LED Fireworks

Clap activated LED firework display for arduino-based micro-controllers.

An ESP32 has been used, but can be swapped out for simpler boards such as the Arduino UNO, nano etc.

### Equipment

- [ESP32 Board](https://www.jaycar.com.au/esp32-main-board-with-wifi-and-bluetooth-communication/p/XC3800) (Can be substituted with Arduino UNO, nano etc.)
- [Microphone Sound Sensor Module](https://www.jaycar.com.au/arduino-compatible-microphone-sound-sensor-module/p/XC4438)
- [2m RGB LED strip (120 LEDs)](https://www.jaycar.com.au/2m-rgb-led-strip-with-120-x-addressable-w2812b-rgb-leds-arduino-mcu-compatible-5v/p/XC4390)
- [Breadboard](https://www.jaycar.com.au/arduino-compatible-breadboard-with-400-tie-points/p/PB8820)

Other equipment such as jumper cables, double sided tape etc. not included above.

### Troubleshooting

#### Lights freeze and/or change to various colors

This can be caused by a power supply problem when many lights are active at once. Use a multimeter to check whether the voltage drops significantly below 5V.

This can be resolved in one of the following ways:

- Replace the power supply with one that can provide a sufficient amount of current without too much voltage drop
- Lower the overall brightness of the strip (use `pixel.setBrightness(n)` in `setup`)
- Lower the maximum amount of worms to prevent too many lights being active at a time
- Reduce the length of the strip

#### Microphone output is always reading LOW/HIGH

The microphone sensitivity can be adjusted with a screwdriver via the potentiometer.

The module includes 2 LEDs.
LED 1 (right side) indicates power, while LED 2 (left side) indicates a 'HIGH' digital signal.

The module should be calibrated such that LED 2 is off.

If LED 2 is on, rotate the potentiometer counter-clockwise just enough until the LED turns off.

If LED 2 is off, but does not turn on when making noise, rotate the potentiometer clockwise until the LED turns on, then rotate slightly backwards until the LED turns off again.
