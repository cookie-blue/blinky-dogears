#include "fade_pattern.h"
#include "pattern_manager.h"
#include "main.h"
#include "config.cpp"
#include <cstdarg>

bool FadePattern::fading = false;
unsigned long FadePattern::fadeStartMillis = 0;

void FadePattern::_start(const Step &currentPatternStep, uint8_t step)
{
    if (step == 0)
        log("fade pattern start");

    fading = true;
    fadeStartMillis = millis();

    for (uint8_t i = 0; i < LED_COUNT; i++)
    {
        DUTY_TYPE startBrightness = ledcRead(ledPins[i]);
        DUTY_TYPE targetBrightness = currentPatternStep.leds[i];
        ledcFade(ledPins[i], startBrightness, targetBrightness, currentPatternStep.duration);
    }
}

void FadePattern::run()
{
    uint8_t pattern = PatternManager::getCurrentPattern();
    uint8_t step = PatternManager::getCurrentStep();
    const Step currentPatternStep = patterns[pattern][step];

    if (!fading)
        _start(currentPatternStep, step);

    if (millis() - fadeStartMillis >= currentPatternStep.duration)
    {
        fading = false;
        PatternManager::nextStep();
    }
}

void FadePattern::reset()
{
    fading = false;
    fadeStartMillis = millis();
}
