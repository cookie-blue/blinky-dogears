#pragma once

#include "config.h"

struct Step
{
    uint8_t leds[LED_COUNT]; // brightness (0–255)
    unsigned int duration;   // milliseconds
    bool fade;               // true = fade, false = instant
};

struct Pattern {
    uint8_t brightness;  // overall brightness modifier for each pattern
    Step steps[MAX_PATTERN_LENGTH];
};
