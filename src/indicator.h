#pragma once

#include "config.h"
#include "log.h"

class Indicator
{
private:
    static unsigned long _previousMillis;
    static bool _isRunning;
    static bool _isOn;
    static uint8_t _count;
    static unsigned int _durationOn;
    static unsigned int _durationOff;

public:
    static void init();
    static void blink(uint8_t count, unsigned int duration);
    static void blink(uint8_t count, unsigned int durationOn, unsigned int durationOff);
    static void run();
};
