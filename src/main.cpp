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

The mapping functions for pallettes and patterns using the entire grid were adapted
from: https://github.com/jasoncoon/led-mapper/blob/main/fastled-map-demo/fastled-map-demo.ino

******************************************************** */

// #define DEBUG_PRINTING_

#define FASTLED_INTERNAL 1 // Suppresses the compilation banner from FastLED
#include <FastLED.h>

#include "mapping.h"
#include "RainbowRipple.h"

#define DATA_PIN1 D1
#define DATA_PIN2 D2
#define DATA_PIN3 D3
#define DATA_PIN4 D4 // Oops! Probably should not have used D4 since now the onboard LED can't be turned off.
#define POT_PIN A0

const uint16_t LEN0 = 168;
const uint16_t LEN1 = 154;
const uint16_t LEN2 = 154;
const uint16_t LEN3 = 84;

CRGBArray<LEN0> leds0;
CRGBArray<LEN1> leds1;
CRGBArray<LEN2> leds2;
CRGBArray<LEN3> leds3;

const uint8_t VELOCITY_MIN = 75;
const uint8_t VELOCITY_MAX = 200;

const uint8_t SATURATION_MIN = 170;
const uint8_t SATURATION_MAX = 255;

uint8_t blend_factor = 20;
uint8_t starting_hue = 0;
uint8_t pattern_number = 0;
uint8_t current_pattern = 0;
uint8_t current_pallette_index = 0;
uint8_t speed = 30;
uint8_t offset = 0; // rotating "base color" used by many of the patterns
uint16_t new_velocity = 0;
uint16_t pot_level = 0;

CRGBPalette16 current_pallette;

uint8_t get_brightness();
void assign_led(uint16_t, CRGB);
void clockwisePalette();
void counterClockwisePalette();
void outwardPalette();
void inwardPalette();
void northPalette();
void northEastPalette();
void eastPalette();
void southEastPalette();
void southPalette();
void southWestPalette();
void westPalette();
void northWestPalette();
void beatsin8Pallette();
void default_pattern();

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
SimplePatternList patterns = {
    // 2D map examples:
    beatsin8Pallette,
    clockwisePalette,
    counterClockwisePalette,
    outwardPalette,
    inwardPalette,
    northPalette,
    northEastPalette,
    eastPalette,
    southEastPalette,
    southPalette,
    southWestPalette,
    westPalette,
    northWestPalette,

    // standard FastLED demo reel examples:
    //  rainbow,
    //  rainbowWithGlitter,
    //  confetti,
    //  sinelon,
    //  juggle,
    //  bpm
};

const uint8_t pattern_count = ARRAY_SIZE(patterns);
uint8_t current_pattern_index = 0; // Index number of which pattern is current

CRGBPalette16 IceColors_p = CRGBPalette16(CRGB::Black, CRGB::Blue, CRGB::Aqua, CRGB::White);
const CRGBPalette16 palettes[] = {
    RainbowColors_p,
    RainbowStripeColors_p,
    CloudColors_p,
    LavaColors_p,
    OceanColors_p,
    ForestColors_p,
    PartyColors_p,
    HeatColors_p,
    IceColors_p,
};

const uint8_t palette_count = ARRAY_SIZE(palettes);

RainbowRipple rr1 = RainbowRipple(leds0, LEN0, random8(), 5, 5);
RainbowRipple rr2 = RainbowRipple(leds1, LEN1, random8(), 17, random16(VELOCITY_MIN, VELOCITY_MAX));
RainbowRipple rr3 = RainbowRipple(leds2, LEN2, random8(), 13, random16(VELOCITY_MIN, VELOCITY_MAX));
RainbowRipple rr4 = RainbowRipple(leds3, LEN3, random8(), 11, random16(VELOCITY_MIN, VELOCITY_MAX));

void setup()
{
    Serial.begin(115200);
    Serial.println("resetting");

    // FastLED.addLeds<APA102>(leds, NUM_LEDS);
    FastLED.addLeds<NEOPIXEL, DATA_PIN1>(leds0, LEN0);
    FastLED.addLeds<NEOPIXEL, DATA_PIN2>(leds1, LEN1);
    FastLED.addLeds<NEOPIXEL, DATA_PIN3>(leds2, LEN2);
    FastLED.addLeds<NEOPIXEL, DATA_PIN4>(leds3, LEN3);

    FastLED.showColor(CRGB::Black);

#ifdef DEBUG_PRINTING_
    Serial.println("Setting pin D1 to red");
    int i;
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
#endif

    FastLED.setBrightness(get_brightness());
    FastLED.show();
}

void loop()
{
    offset = beat8(speed);
    patterns[current_pattern_index]();
    FastLED.setBrightness(get_brightness());
    FastLED.show();

    EVERY_N_SECONDS(30)
    {
        current_pattern_index = (current_pattern_index + 1) % pattern_count;
        if (current_pattern_index == 0)
        {
            current_pallette_index = (current_pallette_index + 1) % palette_count;
        }
    }
}

void loop2()
{
    switch (current_pattern)
    {
    default:
        default_pattern();
        break;
    }

#ifdef DEBUG_PRINTING_
    Serial.print("Brightness: ");
    Serial.println(get_brightness());
#endif
    FastLED.setBrightness(get_brightness());
    FastLED.show();

    EVERY_N_SECONDS(20)
    {
        pattern_number = random8(3);
        switch (pattern_number)
        {
        case 0:
            // Setting the starting hue to a random point around the color wheel.
            // Starting at 0 was... boring.
            // Also, varying the saturation a bit.
            new_velocity = random16(VELOCITY_MIN, VELOCITY_MAX);
            starting_hue = random8();
            rr1.set_hue_and_progression(starting_hue, 2);
            rr1.set_velocity(new_velocity);
            rr1.set_saturation_and_progression(random8(SATURATION_MIN, SATURATION_MAX), 0);
            rr2.set_hue_and_progression(starting_hue, -2);
            rr2.set_velocity(new_velocity);
            rr2.set_saturation_and_progression(random8(SATURATION_MIN, SATURATION_MAX), 0);
            rr3.set_hue_and_progression(starting_hue, 2);
            rr3.set_velocity(new_velocity);
            rr3.set_saturation_and_progression(random8(SATURATION_MIN, SATURATION_MAX), 0);
            rr4.set_hue_and_progression(starting_hue, -2);
            rr4.set_velocity(new_velocity);
            rr4.set_saturation_and_progression(random8(SATURATION_MIN, SATURATION_MAX), 0);
            break;

        case 1:
            // These colors shouldn't sync, so we just start at random points
            // around the color wheel. I didn't want the colors to jump TOO much
            // as they change so I'm limiting hue progression to 1-7. I'm not
            // positive that this is the correct range, but it looks good!
            rr1.set_hue_and_progression(random8(), random(-7, 7));
            rr1.set_velocity(random16(VELOCITY_MIN, VELOCITY_MAX));
            rr1.set_saturation_and_progression(random8(), random(-2, 2));
            rr2.set_hue_and_progression(random8(), random(-7, 7));
            rr2.set_velocity(random16(VELOCITY_MIN, VELOCITY_MAX));
            rr2.set_saturation_and_progression(random8(), random(-2, 2));
            rr3.set_hue_and_progression(random8(), random(-7, 7));
            rr3.set_velocity(random16(VELOCITY_MIN, VELOCITY_MAX));
            rr3.set_saturation_and_progression(random8(), random(-2, 2));
            rr4.set_hue_and_progression(random8(), random(-7, 7));
            rr4.set_velocity(random16(VELOCITY_MIN, VELOCITY_MAX));
            rr4.set_saturation_and_progression(random8(), random(-2, 2));
            break;

        case 2:
            // This is an experiment to vary saturation. Not sure if this will
            // look good or not.
            rr1.set_hue_and_progression(random8(), 0);
            rr1.set_velocity(random16(VELOCITY_MIN, VELOCITY_MAX));
            rr1.set_saturation_and_progression(random8(), random(-7, 7));
            rr2.set_hue_and_progression(random8(), 0);
            rr2.set_velocity(random16(VELOCITY_MIN, VELOCITY_MAX));
            rr2.set_saturation_and_progression(random8(), random(-7, 7));
            rr3.set_hue_and_progression(random8(), 0);
            rr3.set_velocity(random16(VELOCITY_MIN, VELOCITY_MAX));
            rr3.set_saturation_and_progression(random8(), random(-7, 7));
            rr4.set_hue_and_progression(random8(), 0);
            rr4.set_velocity(random16(VELOCITY_MIN, VELOCITY_MAX));
            rr4.set_saturation_and_progression(random8(), random(-7, 7));
            break;

        default:
            // It's safe to do nothing since we'll just repeat the last action.
            break;
        }
    }

    delay(1);
}

void default_pattern()
{
    rr1.Draw(blend_factor);
    rr2.Draw(blend_factor);
    rr3.Draw(blend_factor);
    rr4.Draw(blend_factor);
}

uint8_t get_brightness()
{
#ifdef DEBUG_PRINTING_
    Serial.println(analogRead(POT_PIN));
#endif

    return map(analogRead(POT_PIN), 0, 1023, 0, 255);
}

void assign_led(uint16_t index, CRGB new_color)
{
    if (index < LEN0)
    {
        leds0[index] = new_color;
    }
    else if (index < LEN0 + LEN1)
    {
        leds1[index - LEN0] = new_color;
    }
    else if (index < LEN0 + LEN1 + LEN2)
    {
        leds2[index - LEN0 - LEN1] = new_color;
    }
    else
    {
        leds3[index - LEN0 - LEN1 - LEN2] = new_color;
    }
}

void beatsin8Pallette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[current_pallette_index], coordsX[i], beatsin8(speed * 40, 0, 255, 0, coordsX[i] + coordsY[i])));
    }
}

void clockwisePalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[current_pallette_index], offset + angles[i]));
    }
}

void counterClockwisePalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[current_pallette_index], offset - angles[i]));
    }
}

void outwardPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[current_pallette_index], offset - radii[i]));
    }
}

void inwardPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[current_pallette_index], offset + radii[i]));
    }
}

void northPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[current_pallette_index], offset - coordsY[i]));
    }
}

void northEastPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[current_pallette_index], offset - (coordsX[i] + coordsY[i])));
    }
}

void eastPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[current_pallette_index], offset - coordsX[i]));
    }
}

void southEastPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[current_pallette_index], offset - coordsX[i] + coordsY[i]));
    }
}

void southPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[current_pallette_index], offset + coordsY[i]));
    }
}

void southWestPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[current_pallette_index], offset + coordsX[i] + coordsY[i]));
    }
}

void westPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[current_pallette_index], offset + coordsX[i]));
    }
}

void northWestPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[current_pallette_index], offset + coordsX[i] - coordsY[i]));
    }
}
