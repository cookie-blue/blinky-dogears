#include "fade_pattern.h"

bool FadePattern::_fading = false;
unsigned long FadePattern::_fadeStartMillis = 0;
uint8_t FadePattern::_startBrightness[LED_COUNT];
uint8_t FadePattern::_targetBrightness[LED_COUNT];

void FadePattern::_start(const Step &currentPatternStep)
{
    Log::log("fade pattern start");

    FadePattern::_fading = true;
    FadePattern::_fadeStartMillis = millis();

    for (uint8_t i = 0; i < LED_COUNT; i++)
    {
        FadePattern::_startBrightness[i] = ledcRead(ledPins[i]);
        FadePattern::_targetBrightness[i] = currentPatternStep.leds[i];
    }
}

void FadePattern::run()
{
    uint8_t pattern = PatternManager::getCurrentPattern();
    uint8_t step = PatternManager::getCurrentStep();
    const Step currentPatternStep = patterns[pattern][step];

    if (!FadePattern::_fading)
        FadePattern::_start(currentPatternStep);

    float progress = float(millis() - FadePattern::_fadeStartMillis) / currentPatternStep.duration;

    for (uint8_t i = 0; i < LED_COUNT; i++)
    {
        uint8_t brightness = FadePattern::_startBrightness[i] + (FadePattern::_targetBrightness[i] - FadePattern::_startBrightness[i]) * min(progress, 1.0f);
        LED::write(ledPins[i], brightness);
    }

    if (millis() - FadePattern::_fadeStartMillis >= currentPatternStep.duration)
    {
        FadePattern::_fading = false;
        PatternManager::nextStep();
    }
}

void FadePattern::reset()
{
    FadePattern::_fading = false;
    FadePattern::_fadeStartMillis = millis();
}
