#include <Adafruit_NeoPixel.h>

//ARDUINO
// const int STRIP_PIN = 6;   // Digital pin connected to DIN on the led strip
// const int NUM_PIXELS = 20; // How many individual LEDs are connected on the strip
// const int WORM_LENGTH = 3; // How long a single light of worms is

//ESP32
const int STRIP_PIN = 32;   // Digital pin connected to DIN on the led strip
const int NUM_PIXELS = 120; // How many individual LEDs are connected on the strip
const int WORM_LENGTH = 12; // How long a single light of worms is

const int NUM_MAX_WORMS = 10; // How many worms can be active on the strip at once

Adafruit_NeoPixel pixels(NUM_PIXELS, STRIP_PIN, NEO_GRB + NEO_KHZ800); //set up pixels object

uint32_t red = pixels.Color(100, 0, 0);        // rgb(100,0,0)
uint32_t orange = pixels.Color(100, 50, 0);    // rgb(100,50,0)
uint32_t yellow = pixels.Color(100, 100, 0);   // rgb(100,100,0)
uint32_t green = pixels.Color(0, 100, 0);      // rgb(0,100,0)
uint32_t light_blue = pixels.Color(0, 0, 100); // rgb(0,100,100)
uint32_t blue = pixels.Color(0, 0, 100);       // rgb(0,0,100)
uint32_t indigo = pixels.Color(150, 0, 150);   // rgb(50,0,100)
uint32_t magenta = pixels.Color(150, 0, 150);  // rgb(100,0,100)
uint32_t unlit = pixels.Color(0, 0, 0);        // rgb(0,0,0)

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

class Worm
{
private:
    int head = -1;
    uint32_t color = unlit;

public:
    /**
     * Returns the worm's head position
     */
    int getHead()
    {
        return head;
    }
    /**
     * Returns the worm's color
     */
    uint32_t getColor()
    {
        return color;
    }
    /**
     * Returns whether the worm is currently on the strip
     */
    bool isOnStrip()
    {
        return head >= 0;
    }

    /**
     * Returns whether the worm is past the strip
     */
    bool isPastStrip()
    {
        return (head - WORM_LENGTH) >= NUM_PIXELS;
    }

    /**
     * Positions the worm to start wriggling, and assigns a color
     */
    void start()
    {
        head = 0;
        color = getRandomColor();
    }
    /**
     * Moves the worm forward one space
     */
    void wriggle()
    {
        head += 1;
    }
    /**
     * Resets the worm position off the strip
     */
    void reset()
    {
        head = -1;
    }
};

Worm worms[NUM_MAX_WORMS] = {};

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

    // find the first worm not on the strip and add it to the strip
    for (int i = 0; i < NUM_MAX_WORMS; i++)
    {
        if (!worms[i].isOnStrip())
        {
            worms[i].start();
            break;
        }
    }
    lastStart = millis();
}

/**
 * Fills a worm based on the provided head index and colour
 */
void fillWorm(int headIdx, uint32_t color)
{
    int count = headIdx < WORM_LENGTH - 1 ? headIdx + 1 : WORM_LENGTH;
    int theoreticalWormTail = headIdx - WORM_LENGTH + 1;
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
void tickWorms()
{
    for (int i = 0; i < NUM_MAX_WORMS; i++)
    {
        if (worms[i].isOnStrip())
        {
            fillWorm(worms[i].getHead(), worms[i].getColor());
            worms[i].wriggle();
        }
        if (worms[i].isPastStrip())
        {
            worms[i].reset();
        }
    }
    pixels.show();
}
