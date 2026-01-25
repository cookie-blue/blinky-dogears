#include "fade_pattern.h"

bool FadePattern::fading = false;
unsigned long FadePattern::fadeStartMillis = 0;
uint8_t startBrightness[LED_COUNT];
uint8_t targetBrightness[LED_COUNT];

void FadePattern::_start(const Step &currentPatternStep)
{
    Log::log("fade pattern start");

    fading = true;
    fadeStartMillis = millis();

    for (uint8_t i = 0; i < LED_COUNT; i++)
    {
        startBrightness[i] = ledcRead(ledPins[i]);
        targetBrightness[i] = currentPatternStep.leds[i];
    }
}

void FadePattern::run()
{
    uint8_t pattern = PatternManager::getCurrentPattern();
    uint8_t step = PatternManager::getCurrentStep();
    const Step currentPatternStep = patterns[pattern][step];

    if (!fading)
        _start(currentPatternStep);

    float progress = float(millis() - fadeStartMillis) / currentPatternStep.duration;

    for (uint8_t i = 0; i < LED_COUNT; i++)
    {
        uint8_t brightness = startBrightness[i] + (targetBrightness[i] - startBrightness[i]) * min(progress, 1.0f);
        DUTY_TYPE gammaValue = PGM_READ_DUTY(&gammaMap[brightness]);
        ledcWrite(ledPins[i], gammaValue);
    }

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
