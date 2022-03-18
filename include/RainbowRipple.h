/* ********************************************************

RainbowRipple

Unlike the rainbowchase that's mostly dark except for the traveling line of dots,
this will be mostly lit rainbows with a ripple of black. Black dots will start
at the ends and move up and down the strips.

Fingers crossed that it will look decent.

******************************************************** */

#ifndef RAINBOWRIPPLE_H_
#define RAINBOWRIPPLE_H_

// #define RAINBOWRIPPLE_H_DEBUG

#include <FastLED.h>
#define FASTLED_INTERNAL

#include "ledhelpers.h"

#define LEDS_PER_SEGMENT 14

class RainbowRipple
{
    int pos1, pos2;
    CRGB *leds;
    uint8_t hue;
    uint8_t hue_progression;
    int led_count;
    unsigned long velocity;
    unsigned long next_draw;
    int direction;

public:
    RainbowRipple(CRGB *, int, uint8_t, uint8_t, unsigned long);
    void Draw(void);
};

RainbowRipple::RainbowRipple(CRGB *leds, int led_count, uint8_t hue, uint8_t hue_progression, unsigned long velocity)
    : leds(leds), led_count(led_count), hue(hue), hue_progression(hue_progression), velocity(velocity)
{
    direction = 1;
    next_draw = 0;
    pos1 = 0;
    pos2 = led_count - 1;
}

void RainbowRipple::Draw(void)
{
    if (millis() > next_draw)
    {
        int i;
        CRGB temp_hue;

        // Determine new hue
        hue += hue_progression;
        temp_hue = CHSV(hue, 255, 255);

        // Blend the current strip to the new colors
        for (i = 0; i < led_count; i++)
        {
            leds[i] = blend(leds[i], temp_hue, 30);
        }
        leds[pos1] = CRGB::Black;
        leds[pos2] = CRGB::Black;

        direction = new_direction(direction, pos1, led_count);
        pos1 += direction;
        pos2 -= direction;

        next_draw = millis() + velocity;
    }
}
#endif