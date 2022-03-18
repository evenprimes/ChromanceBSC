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

#include <FastLED.h>
#define FASTLED_INTERNAL

// #include "SimpleChase.h"
#include "RainbowChase.h"

#define DATA_PIN1 D1
#define DATA_PIN2 D2
#define DATA_PIN3 D3
#define DATA_PIN4 D4

#define LEN0 168
#define LEN1 154
#define LEN2 154
#define LEN3 84

// int lengths[] = {154, 168, 84, 154}; // Strips are different lengths because I am a dumb
uint8_t brightness = 255;

CRGBArray<LEN0> leds0;
CRGBArray<LEN1> leds1;
CRGBArray<LEN2> leds2;
CRGBArray<LEN3> leds3;

// CRGB leds0[LEN0];
// CRGB leds1[LEN1];
// CRGB leds2[LEN2];
// CRGB leds3[LEN3];

RainbowChase stripred = RainbowChase(leds0, random8(), LEN0, 50);
RainbowChase stripblue = RainbowChase(leds1, random8(), LEN1, 40);
RainbowChase stripgreen = RainbowChase(leds2, random8(), LEN2, 75);
RainbowChase stripviolet = RainbowChase(leds3, random8(), LEN3, 100);

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
        leds0[i] = CRGB::Red;
    }

    for (i = 0; i < LEN1; i++)
    {
        leds1[i] = CRGB::Blue;
    }

    for (i = 0; i < LEN2; i++)
    {
        leds2[i] = CRGB::Green;
    }

    for (i = 0; i < LEN3; i++)
    {
        leds3[i] = CRGB::Violet;
    }

    FastLED.setBrightness(brightness);
    FastLED.show();
}

void loop()
{
    stripred.Draw();
    stripblue.Draw();
    stripgreen.Draw();
    stripviolet.Draw();

    FastLED.setBrightness(brightness);
    FastLED.show();

    delay(1);
}
