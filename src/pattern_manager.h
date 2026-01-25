#pragma once

#include "pattern_manager.h"
#include "instant_pattern.h"
#include "fade_pattern.h"
#include <cstdlib>
#include <stdint.h>

class PatternManager
{
private:
    static bool poweredOn;
    static uint8_t currentPattern;
    static uint8_t currentStep;
    static uint8_t numPatterns;
    static uint8_t patternLengths[];
    static void _switchPattern();
    static void _calculatePatternLengths();

public:
    static void init();
    static void runPattern();
    static void prevPattern();
    static void nextPattern();
    static void setPattern(uint8_t newPattern);
    static void on();
    static void off();
    static void override(uint8_t brightness);
    static uint8_t getCurrentPattern();
    static uint8_t getCurrentStep();
    static void nextStep();
};
