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

The mapping functions for palettes and patterns using the entire grid were adapted
from: https://github.com/jasoncoon/led-mapper/blob/main/fastled-map-demo/fastled-map-demo.ino

******************************************************** */

// #define DEBUG_PRINTING_

#include "main.h"

#include "mapping.h"
#include "RainbowRipple.h"
#include "mqttled.h"

CRGBArray<LEN0> leds0;
CRGBArray<LEN1> leds1;
CRGBArray<LEN2> leds2;
CRGBArray<LEN3> leds3;

uint8_t blend_factor = 20;
uint8_t starting_hue = 0;
uint8_t pattern_number = 0;
uint8_t current_pattern = 0;
// uint8_t speed = 30;
uint8_t offset = 0; // rotating "base color" used by many of the patterns
uint16_t new_velocity = 0;

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

    current_pattern = random_pattern();
}

void loop()
{
    if (WiFi.status() != WL_CONNECTED)
    {
        wifi_init();
    }

    // Handle any MQTT updates
    mqtt_client.loop();

    switch (g_mode)
    {
    case (LEDMODE_INDIVIDUAL):
        mode_individual();
        break;
    case (LEDMODE_UNIFIED):
        mode_unified();
        break;
    default:
        mode_random();
        break;
    }

    FastLED.setBrightness(get_brightness());
    FastLED.show();

#ifdef MQTTLED_DEBUG
    EVERY_N_SECONDS(2)
    {
        Serial.printf("Mode: %d  Palette: %d  Pattern: %d\n", g_mode, g_palette, g_pattern);
    }
#endif
}

void mode_individual()
{
    rr1.Draw(blend_factor);
    rr2.Draw(blend_factor);
    rr3.Draw(blend_factor);
    rr4.Draw(blend_factor);

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

void mode_unified()
{
    // Run the current palette and pattern
    offset = beat8(speed);

    if (g_pattern == LEDPATTERN_randomizePalette)
    {
        patterns[current_pattern]();
    }
    else
    {
        patterns[g_pattern]();
    }
    FastLED.setBrightness(get_brightness());
    FastLED.show();

    EVERY_N_SECONDS(3)
    {
        current_pattern = random_pattern();
#ifdef DEBUG_PRINTING_
        Serial.printf("New random pattern: %d\n", current_pattern);
#endif
    }
}

void mode_random()
{
    // Update the LEDs
    mode_unified();

    // Every 30 seconds, update the pattern
    EVERY_N_SECONDS(30)
    {
        g_palette = random8(palette_count);

        g_pattern = random_pattern();
    }
}

uint8_t random_pattern()
{
    // 0 is a non-pattern sentinal value
    return random8(pattern_count - 1) + 1;
}

uint8_t get_brightness()
{
    // #ifdef DEBUG_PRINTING_
    //     Serial.print("Brightness: ");
    //     Serial.println(get_brightness());
    // #endif

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

void randomizePalette()
{
}

void beatsin8Palette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[g_palette], coordsX[i], beatsin8(speed * 40, 0, 255, 0, coordsX[i] + coordsY[i])));
    }
}

void clockwisePalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[g_palette], offset + angles[i]));
    }
}

void counterClockwisePalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[g_palette], offset - angles[i]));
    }
}

void outwardPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[g_palette], offset - radii[i]));
    }
}

void inwardPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[g_palette], offset + radii[i]));
    }
}

void northPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[g_palette], offset - coordsY[i]));
    }
}

void northEastPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[g_palette], offset - (coordsX[i] + coordsY[i])));
    }
}

void eastPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[g_palette], offset - coordsX[i]));
    }
}

void southEastPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[g_palette], offset - coordsX[i] + coordsY[i]));
    }
}

void southPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[g_palette], offset + coordsY[i]));
    }
}

void southWestPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[g_palette], offset + coordsX[i] + coordsY[i]));
    }
}

void westPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[g_palette], offset + coordsX[i]));
    }
}

void northWestPalette()
{
    for (uint16_t i = 0; i < NUM_LEDS; i++)
    {
        assign_led(i, ColorFromPalette(palettes[g_palette], offset + coordsX[i] - coordsY[i]));
    }
}
