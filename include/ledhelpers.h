#ifndef LEDHELPERS_H_
#define LEDHELPERS_H_

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

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
