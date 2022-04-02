#ifndef MAIN_H_
#define MAIN_H_

#define FASTLED_INTERNAL 1 // Suppresses the compilation banner from FastLED
#include <FastLED.h>
#include "ledhelpers.h"

uint8_t get_brightness();
void assign_led(uint16_t, CRGB);
void randomizePalette();
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
void beatsin8Palette();

void mode_individual();
void mode_unified();
void mode_random();

uint8_t random_pattern();

#define DATA_PIN1 D1
#define DATA_PIN2 D2
#define DATA_PIN3 D3
#define DATA_PIN4 D4 // Oops! Probably should not have used D4 since now the onboard LED can't be turned off.
#define POT_PIN A0

const uint16_t LEN0 = 168;
const uint16_t LEN1 = 154;
const uint16_t LEN2 = 154;
const uint16_t LEN3 = 84;

const uint8_t VELOCITY_MIN = 75;
const uint8_t VELOCITY_MAX = 200;

const uint8_t SATURATION_MIN = 170;
const uint8_t SATURATION_MAX = 255;

typedef void (*SimplePatternList[])();
SimplePatternList patterns = {
    randomizePalette,
    beatsin8Palette,
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
#endif
