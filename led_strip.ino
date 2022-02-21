#include <Adafruit_NeoPixel.h>

//ARDUINO
// const int STRIP_PIN = 6;      // Digital pin connected to DIN on the led strip
// const int NUM_PIXELS = 20;    // How many individual LEDs are connected on the strip
// const int WORM_LENGTH = 3;    // How long a single light of worms is
// const int MICROPHONE_PIN = 2; // Digital pin connected to D0 for the microphone

//ESP32
const int STRIP_PIN = 32;      // Digital pin connected to DIN on the led strip
const int NUM_PIXELS = 120;    // How many individual LEDs are connected on the strip
const int WORM_LENGTH = 12;    // How long a single light of worms is
const int MICROPHONE_PIN = 33; // Digital pin connected to D0 for the microphone

const int DELAY = 50; // Time (in ms) to pause between ticks

Adafruit_NeoPixel pixels(NUM_PIXELS, STRIP_PIN, NEO_GRB + NEO_KHZ800); //set up pixels object

uint32_t red = pixels.Color(100, 0, 0);        // rgb(100,0,0)
uint32_t orange = pixels.Color(100, 50, 0);    // rgb(100,50,0)
uint32_t yellow = pixels.Color(100, 100, 0);   // rgb(100,100,0)
uint32_t green = pixels.Color(0, 100, 0);      // rgb(0,100,0)
uint32_t light_blue = pixels.Color(0, 0, 100); // rgb(0,100,100)
uint32_t blue = pixels.Color(0, 0, 100);       // rgb(0,0,100)
uint32_t indigo = pixels.Color(150, 0, 150);   // rgb(50,0,100)
uint32_t magenta = pixels.Color(150, 0, 150);  // rgb(100,0,100)

const int NUM_COLOURS = 8;
uint32_t colorList[NUM_COLOURS]{red, orange, yellow, green, light_blue, blue, indigo, magenta};

/**
  * returns a random colour from a pre-defined list
  */
uint32_t getRandomColor()
{
  int index = random(NUM_COLOURS);
  return colorList[index];
}

typedef struct
{
  String name;
  int *head;
  uint32_t *color;
} Worm;

const int NUM_MAX_WORMS = 3;

int head0 = -1;
int head1 = -1;
int head2 = -1;
uint32_t color0 = getRandomColor();
uint32_t color1 = getRandomColor();
uint32_t color2 = getRandomColor();

Worm worms[NUM_MAX_WORMS] = {
    {"bob", &head0, &color0},
    {"myrtle", &head1, &color1},
    {"bear", &head2, &color2},
};

/**
 * Inbuilt Arduino setup
 */
void setup()
{
  pinMode(MICROPHONE_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(MICROPHONE_PIN), startWorm, RISING);
  pixels.begin(); // INITIALIZE neopixel strip object
  Serial.begin(9600);
}

/**
 * Inbuilt Arduino loop
 */
void loop()
{
  tickLights();
  delay(DELAY);
}

volatile int lastStart = 0;

/**
 * Handles settings a worm head to pixel 0 and assigning it a new colour.
 * Uses a debounce implementation to prevent starting worms too close together
 * as a result of fluctuating triggers.
 */
void startWorm()
{
  if (millis() - lastStart <= 300)
  {
    return;
  }

  for (int i = 0; i < NUM_MAX_WORMS; i++)
  {
    Worm worm = worms[i];
    if (*worm.head == -1)
    {
      *worm.head = 0;
      *worm.color = getRandomColor();
      lastStart = millis();
      return;
    }
  }
}

/**
 * Fills a worm based on the provided head index and colour
 */
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

/**
 * Handles checking each worm to fill, and incrementing the worm where necessary.
 */
void tickLights()
{

  for (int i = 0; i < NUM_MAX_WORMS; i++)
  {
    Worm worm = worms[i];
    if (*worm.head >= 0)
    {
      fillWorm(*worm.head, *worm.color);
      *worm.head += 1;
    }
    if ((*worm.head - WORM_LENGTH) >= NUM_PIXELS)
    {
      *worm.head = -1;
    }
  }
  pixels.show();
}
