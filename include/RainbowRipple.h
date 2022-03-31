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

#define FASTLED_INTERNAL 1 // Suppresses the compilation banner from FastLED
#include <FastLED.h>

#include "ledhelpers.h"

const uint16_t LEDS_PER_SEGMENT = 14;

class RainbowRipple
{
    uint16_t pos1, pos2;
    CRGB *leds;
    uint8_t hue, saturation;
    int8_t hue_progression, sat_progression;
    uint16_t led_count;
    uint16_t velocity;
    unsigned long next_draw;
    int8_t direction;

public:
    RainbowRipple(CRGB *, uint16_t, uint8_t, int8_t, uint16_t);
    void Draw(int8_t);
    void set_hue_and_progression(uint8_t, int8_t);
    void set_saturation_and_progression(uint8_t, int8_t);
    // void set_hue(uint8_t);
    // void set_hue_progression(int8_t);
    void set_velocity(unsigned long);
    void set_hue_and_saturation(uint8_t, int8_t, uint8_t, int8_t);
};

RainbowRipple::RainbowRipple(CRGB *leds, uint16_t led_count, uint8_t hue, int8_t hue_progression, uint16_t velocity)
    : leds(leds), hue(hue), hue_progression(hue_progression), led_count(led_count), velocity(velocity)
{
    direction = 1;
    next_draw = 0;
    pos1 = 0;
    pos2 = led_count - 1;
    saturation = 255;
    sat_progression = 0;
}

void RainbowRipple::Draw(int8_t blend_factor = 30)
{
    if (millis() > next_draw)
    {
        int i;
        CRGB temp_hue;

        // Determine new hue
        hue += hue_progression;
        saturation += sat_progression;
        temp_hue = CHSV(hue, saturation, 255);

        // Blend the current strip to the new colors
        for (i = 0; i < led_count; i++)
        {
            leds[i] = blend(leds[i], temp_hue, blend_factor);
        }
        leds[pos1] = CRGB::Black;
        leds[pos2] = CRGB::Black;

        direction = new_direction(direction, pos1, led_count);
        pos1 += direction;
        pos2 -= direction;

        next_draw = millis() + velocity;
    }
}

void RainbowRipple::set_hue_and_progression(uint8_t new_hue, int8_t new_hue_progression)
{
    hue = new_hue;
    hue_progression = new_hue_progression;
}

void RainbowRipple::set_saturation_and_progression(uint8_t new_sat, int8_t new_sat_progression)
{
    saturation = new_sat;
    sat_progression = new_sat_progression;
}

void RainbowRipple::set_hue_and_saturation(uint8_t new_hue, int8_t new_hue_progression, uint8_t new_sat, int8_t new_sat_progression)
{
    hue = new_hue;
    hue_progression = new_hue_progression;
    saturation = new_sat;
    sat_progression = new_sat_progression;
}

void RainbowRipple::set_velocity(unsigned long new_velocity)
{
    velocity = new_velocity;
}
#endif
