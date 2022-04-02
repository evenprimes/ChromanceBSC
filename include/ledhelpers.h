#ifndef LEDHELPERS_H_
#define LEDHELPERS_H_

#include <Arduino.h>

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

// Modes
const int8_t LEDMODE_INDIVIDUAL = 0;
const int8_t LEDMODE_UNIFIED = 1;
const int8_t LEDMODE_RANDOM = 2;

// Patterns

const int8_t LEDPATTERN_randomizePalette = 0;
const int8_t LEDPATTERN_beatsin8Palette = 1;
const int8_t LEDPATTERN_clockwisePalette = 2;
const int8_t LEDPATTERN_counterClockwisePalette = 3;
const int8_t LEDPATTERN_outwardPalette = 4;
const int8_t LEDPATTERN_inwardPalette = 5;
const int8_t LEDPATTERN_northPalette = 6;
const int8_t LEDPATTERN_northEastPalette = 7;
const int8_t LEDPATTERN_eastPalette = 8;
const int8_t LEDPATTERN_southEastPalette = 9;
const int8_t LEDPATTERN_southPalette = 10;
const int8_t LEDPATTERN_southWestPalette = 11;
const int8_t LEDPATTERN_westPalette = 12;
const int8_t LEDPATTERN_northWestPalette = 13;

// Palettes 
const int8_t LEDPALETTE_RainbowColors = 0;
const int8_t LEDPALETTE_RainbowStripeColors = 1;
const int8_t LEDPALETTE_CloudColors = 2;
const int8_t LEDPALETTE_LavaColors = 3;
const int8_t LEDPALETTE_OceanColors = 4;
const int8_t LEDPALETTE_ForestColors = 5;
const int8_t LEDPALETTE_PartyColors = 6;
const int8_t LEDPALETTE_HeatColors = 7;
const int8_t LEDPALETTE_IceColors = 8;

int8 g_mode = LEDMODE_INDIVIDUAL;
int8 g_pattern = LEDPATTERN_randomizePalette;
int8 g_palette = LEDPALETTE_RainbowColors;

int new_direction(int current_direction, int current_position, int led_count)
{
    if (current_direction == 1 && current_position == led_count - 1)
    {
        return -1;
    }
    else if (current_direction == -1 && current_position == 0)
    {
        return 1;
    }

    return current_direction;
}

#endif
