#ifndef RAINBOWCHASE_H_
#define RAINBOWCHASE_H_

// #define RAINBOWCHASE_H_DEBUG

#include <FastLED.h>
#define FASTLED_INTERNAL

#define LEDS_PER_SEGMENT 14

class RainbowChase
{
    int position;
    CRGB *leds;
    uint8_t hue;
    uint8_t hue_progression;
    int led_count;
    unsigned long velocity;
    unsigned long next_draw;
    int direction;


public:
    RainbowChase(CRGB *, uint8_t, uint8_t, int, unsigned long);
    void Draw(uint8_t);
};

RainbowChase::RainbowChase(CRGB *ledarray, uint8_t color, uint8_t progression, int count, unsigned long velocity)
    : leds(ledarray), hue(color), hue_progression(progression), led_count(count), velocity(velocity)
{
    position = 0;
    next_draw = 0;
    direction = 1;
}

void RainbowChase::Draw(uint8_t fade_factor=40)
{
    unsigned long now = millis();
    if (now > next_draw)
    {
        // Fade the existing pixels
        for (int i = 0; i < led_count; i++)
        {
            leds[i].fadeToBlackBy(fade_factor);
        }

        // Add a new head dot
        leds[position] = CHSV(hue, 255, 255);

        // Increment the head
        if (direction == 1 && position == led_count - 1)
        {
            direction = -1;
        }
        else if (direction == -1 && position == 0)
        {
            direction = 1;
        }
        position += direction;

        if (position % LEDS_PER_SEGMENT == 0)
        {
            hue += hue_progression;
        }


        next_draw += velocity;
#ifdef RAINBOWCHASE_H_DEBUG
        uint8_t segment = position / LEDS_PER_SEGMENT;

        Serial.print("position: ");
        Serial.print(position);
        Serial.print("  segment: ");
        Serial.print(segment);
        Serial.print("  hue: ");
        Serial.print(hue);
        Serial.println(" ");
#endif
    }
}

#endif
