#include "Worm.h"

//ARDUINO
// const int MICROPHONE_PIN = 2; // Digital pin connected to D0 for the microphone

//ESP32
const int MICROPHONE_PIN = 33; // Digital pin connected to D0 for the microphone

/**
 * Inbuilt Arduino setup
 */
void setup()
{
  pinMode(MICROPHONE_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(MICROPHONE_PIN), startWorm, RISING);
  pixels.begin();
  pixels.setBrightness(50);
  Serial.begin(9600);
}

/**
 * Inbuilt Arduino loop
 */
void loop()
{
  tickWorms();
  delay(50);
}
