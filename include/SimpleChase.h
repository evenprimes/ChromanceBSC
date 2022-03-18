#ifndef SIMPLECHASE_H_
#define SIMPLECHASE_H_

#include <FastLED.h>
#define FASTLED_INTERNAL

class SimpleChase
{
    int position;
    CRGB *leds;
    CRGB color;
    int led_count;
    unsigned long velocity;
    unsigned long next_draw;
    int direction;

public:
    SimpleChase(CRGB *, CRGB, int, unsigned long);
    void Draw(void);
};

SimpleChase::SimpleChase(CRGB *ledarray, CRGB color, int count, unsigned long velocity)
    : leds(ledarray), color(color), led_count(count), velocity(velocity)
{
    position = 0;
    next_draw = 0;
    direction = 1;
}

void SimpleChase::Draw(void)
{
    unsigned long now = millis();
    if (now > next_draw)
    {
        // Fade the existing pixels
        for (int i = 0; i < led_count; i++)
        {
            leds[i].fadeToBlackBy(40);
        }

        // Add a new head dot
        leds[position] = color;

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

        next_draw += velocity;
        // Serial.println(position);
    }
}

#endif
