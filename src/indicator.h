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
#ifdef STATUS_LED_MODE_RGB
    static uint8_t _color[3];
    static rmt_data_t _ledData[24];
    static void buildLedData(bool isOn, const uint8_t color[3]);
#endif

public:
    static void init();
    static void blink(uint8_t count, unsigned int duration);
    static void blink(uint8_t count, unsigned int durationOn, unsigned int durationOff);
#ifdef STATUS_LED_MODE_RGB
    static void blink(uint8_t count, unsigned int duration, uint8_t color[]);
    static void blink(uint8_t count, unsigned int durationOn, unsigned int durationOff, uint8_t color[]);
#endif
    static void on();
    static void off();
    static void set(uint8_t state);
#ifdef STATUS_LED_MODE_RGB
    static void set(uint8_t state, uint8_t color[]);
#endif
    static void run();
};
