#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

#define LED_COUNT 2

struct Step
{
  uint8_t leds[LED_COUNT]; // brightness (0–255)
  unsigned int duration;   // milliseconds
  bool fade;               // true = fade, false = instant
};

void log(const char *format, ...);

#endif
