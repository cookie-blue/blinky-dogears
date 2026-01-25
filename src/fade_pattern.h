#pragma once

#include "config.h"
#include "log.h"
#include "led.h"
#include "pattern_manager.h"
#include <cstdarg>

class FadePattern
{
private:
    static bool _fading;
    static unsigned long _fadeStartMillis;
    static uint8_t _startBrightness[LED_COUNT];
    static uint8_t _targetBrightness[LED_COUNT];

    static void _start(const Step &currentPatternStep);

public:
    static void run();
    static void reset();
};
