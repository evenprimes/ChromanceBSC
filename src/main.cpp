/* ********************************************************

ChromanceBSC

A FastLED and PlatformIO based version of Chromance.

v1 is local only, with no WiFi support

Blue and bacl on my bottom 2, blue on right as facing, and green on the right as well


My color = Zack color
Yellow - Red
Blacl - Black
Green - Green
Blue - Blue

******************************************************** */

// #define DEBUG_PRINTING_

#define FASTLED_INTERNAL
#include <FastLED.h>

#include "RainbowRipple.h"

#define DATA_PIN1 D1 // Oops! Probably should not have used D1 since now the onboard LED can't be turned off.
#define DATA_PIN2 D2
#define DATA_PIN3 D3
#define DATA_PIN4 D4
#define POT_PIN A0

#define LEN0 168
#define LEN1 154
#define LEN2 154
#define LEN3 84

#define VELOCITY_MIN 75
#define VELOCITY_MAX 200

// int lengths[] = {154, 168, 84, 154}; // Strips are different lengths because I am a dumb
uint8_t brightness = 255;
uint8_t blend_factor = 20;
uint8_t sync = 0;
unsigned long new_velocity = 0;
uint8_t starting_hue = 0;
uint16_t pot_level = 0;

CRGBArray<LEN0> leds0;
CRGBArray<LEN1> leds1;
CRGBArray<LEN2> leds2;
CRGBArray<LEN3> leds3;

RainbowRipple rr1 = RainbowRipple(leds0, LEN0, random8(), 5, 20);
RainbowRipple rr2 = RainbowRipple(leds1, LEN1, random8(), 17, random16(VELOCITY_MIN, VELOCITY_MAX));
RainbowRipple rr3 = RainbowRipple(leds2, LEN2, random8(), 13, random16(VELOCITY_MIN, VELOCITY_MAX));
RainbowRipple rr4 = RainbowRipple(leds3, LEN3, random8(), 11, random16(VELOCITY_MIN, VELOCITY_MAX));

uint8_t get_brightness();

void setup()
{
    int i;

    Serial.begin(115200);
    Serial.println("resetting");

    // FastLED.addLeds<APA102>(leds, NUM_LEDS);
    FastLED.addLeds<NEOPIXEL, DATA_PIN1>(leds0, LEN0);
    FastLED.addLeds<NEOPIXEL, DATA_PIN2>(leds1, LEN1);
    FastLED.addLeds<NEOPIXEL, DATA_PIN3>(leds2, LEN2);
    FastLED.addLeds<NEOPIXEL, DATA_PIN4>(leds3, LEN3);

    FastLED.showColor(CRGB::Black);

    // Serial.println("Setting pin D1 to red");
    for (i = 0; i < LEN0; i++)
    {
        leds0[i] = CRGB::Black;
    }

    for (i = 0; i < LEN1; i++)
    {
        leds1[i] = CRGB::Black;
    }

    for (i = 0; i < LEN2; i++)
    {
        leds2[i] = CRGB::Black;
    }

    for (i = 0; i < LEN3; i++)
    {
        leds3[i] = CRGB::Black;
    }

    brightness = get_brightness();
    FastLED.setBrightness(brightness);
    FastLED.show();
}

void loop()
{
    rr1.Draw(blend_factor);
    rr2.Draw(blend_factor);
    rr3.Draw(blend_factor);
    rr4.Draw(blend_factor);

    brightness = get_brightness();
    // #ifdef DEBUG_PRINTING_
    //     Serial.println(brightness);
    // #endif
    FastLED.setBrightness(brightness);
    FastLED.show();

    EVERY_N_SECONDS(20)
    {
        if (sync)
        {
            // Setting the starting hue to a random point around the color wheel.
            // Starting at 0 was... boring.
            new_velocity = random16(VELOCITY_MIN, VELOCITY_MAX);
            starting_hue = random8();
            rr1.set_hue_and_progression(starting_hue, 2);
            rr1.set_velocity(new_velocity);
            rr2.set_hue_and_progression(starting_hue, -2);
            rr2.set_velocity(new_velocity);
            rr3.set_hue_and_progression(starting_hue, 2);
            rr3.set_velocity(new_velocity);
            rr4.set_hue_and_progression(starting_hue, -2);
            rr4.set_velocity(new_velocity);
        }
        else
        {
            // These colors shouldn't sync, so we just start at random points
            // around the color wheel. I didn't want the colors to jump TOO much
            // as they change so I'm limiting hue progression to 1-7. I'm not
            // positive that this is the correct range, but it looks good!
            rr1.set_hue_and_progression(random8(), random(-7, 7));
            rr1.set_velocity(random16(VELOCITY_MIN, VELOCITY_MAX));
            rr2.set_hue_and_progression(random8(), random(-7, 7));
            rr2.set_velocity(random16(VELOCITY_MIN, VELOCITY_MAX));
            rr3.set_hue_and_progression(random8(), random(-7, 7));
            rr3.set_velocity(random16(VELOCITY_MIN, VELOCITY_MAX));
            rr4.set_hue_and_progression(random8(), random(-7, 7));
            rr4.set_velocity(random16(VELOCITY_MIN, VELOCITY_MAX));
        }

        sync = !sync;
    }

    delay(1);
}

uint8_t get_brightness(void)
{
#ifdef DEBUG_PRINTING_
    Serial.println(analogRead(POT_PIN));
#endif

    return map(analogRead(POT_PIN), 0, 1023, 0, 255);
}
