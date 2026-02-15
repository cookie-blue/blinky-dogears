#pragma once

#include "config.h"

struct Step
{
    uint8_t leds[LED_COUNT]; // brightness (0–255)
    unsigned int duration;   // milliseconds
    bool fade;               // true = fade, false = instant
};
