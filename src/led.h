#pragma once

#include "config.h"
#include "pattern_manager.h"

class LED
{
private:
    static uint8_t _brightness[LED_COUNT];
    static uint8_t _getPinIndex(uint8_t ledPin);

public:
    static void write(uint8_t ledPin, uint8_t brightness);
    static uint8_t getBrightness(uint8_t ledPin);
};
