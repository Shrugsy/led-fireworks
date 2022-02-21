#include <Adafruit_NeoPixel.h>

//ARDUINO
// const int STRIP_PIN = 6; // Digital pin connected to DIN on the led strip
// const int NUM_PIXELS = 20; // How many individual LEDs are connected on the strip
// const int WORM_LENGTH = 3; // How long a single light of worms is
// const int MICROPHONE_PIN = 2; // Digital pin connected to D0 for the microphone

//ESP32
const int STRIP_PIN = 32;      // Digital pin connected to DIN on the led strip
const int NUM_PIXELS = 120;    // How many individual LEDs are connected on the strip
const int WORM_LENGTH = 12;    // How long a single light of worms is
const int MICROPHONE_PIN = 33; // Digital pin connected to D0 for the microphone

const int DELAY = 50; // Time (in ms) to pause between ticks

Adafruit_NeoPixel pixels(NUM_PIXELS, STRIP_PIN, NEO_GRB + NEO_KHZ800); //set up pixels object

uint32_t magenta = pixels.Color(150, 0, 150);  // rgb(150,0,150)
uint32_t green = pixels.Color(0, 100, 0);      // rgb(0,100,0)
uint32_t red = pixels.Color(100, 0, 0);        // rgb(100,0,0)
uint32_t blue = pixels.Color(0, 0, 100);       // rgb(0,0,100)
uint32_t light_blue = pixels.Color(0, 0, 100); // rgb(0,100,255)
uint32_t off = pixels.Color(0, 0, 0);          // rgb(0,0,0)

const int NUM_COLOURS = 5;
uint32_t colorList[NUM_COLOURS]{magenta, green, red, blue, light_blue};

/**
  * returns a random colour from a pre-defined list
  */
uint32_t getRandomColor()
{
  int index = random(NUM_COLOURS);
  return colorList[index];
}

int worm_A_head = -1;
int worm_B_head = -1;
int worm_C_head = -1;
uint32_t worm_A_color = getRandomColor();
uint32_t worm_B_color = getRandomColor();
uint32_t worm_C_color = getRandomColor();

void setup()
{
  pinMode(MICROPHONE_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(MICROPHONE_PIN), startWorm, RISING);
  pixels.begin(); // INITIALIZE neopixel strip object
  Serial.begin(9600);
}

void loop()
{
  tickLights();
  delay(DELAY);
}

volatile int lastStart = 0;

void startWorm()
{
  if (millis() - lastStart <= 300)
  {
    return;
  }

  if (worm_A_head == -1)
  {
    worm_A_head = 0;
  }
  else if (worm_B_head == -1)
  {
    worm_B_head = 0;
  }
  else if (worm_C_head == -1)
  {
    worm_C_head = 0;
  }
  lastStart = millis();
}

void fillWorm(int wormHeadIdx, uint32_t color)
{
  int count = wormHeadIdx < WORM_LENGTH - 1 ? wormHeadIdx + 1 : WORM_LENGTH;
  int theoreticalWormTail = wormHeadIdx - WORM_LENGTH + 1;
  int wormTail = theoreticalWormTail < 0 ? 0 : theoreticalWormTail;
  if (wormTail - 1 >= 0)
  {
    pixels.setPixelColor(wormTail - 1, 0, 0, 0);
  }

  pixels.fill(color, wormTail, count);
}

void tickLights()
{

  if (worm_A_head >= 0)
  {
    fillWorm(worm_A_head, worm_A_color);
    worm_A_head += 1;
  }
  if (worm_B_head >= 0)
  {
    fillWorm(worm_B_head, worm_B_color);
    worm_B_head += 1;
  }
  if (worm_C_head >= 0)
  {
    fillWorm(worm_C_head, worm_C_color);
    worm_C_head += 1;
  }

  if ((worm_A_head - WORM_LENGTH) >= NUM_PIXELS)
  {
    worm_A_head = -1;
    worm_A_color = getRandomColor();
  }
  if ((worm_B_head - WORM_LENGTH) >= NUM_PIXELS)
  {
    worm_B_head = -1;
    worm_B_color = getRandomColor();
  }
  if ((worm_C_head - WORM_LENGTH) >= NUM_PIXELS)
  {
    worm_C_head = -1;
    worm_C_color = getRandomColor();
  }

  pixels.show();
}
