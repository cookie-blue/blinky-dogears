#include "instant_pattern.h"

unsigned long InstantPattern::previousMillis = 0;

void InstantPattern::run()
{
    uint8_t pattern = PatternManager::getCurrentPattern();
    uint8_t step = PatternManager::getCurrentStep();
    const Step currentPatternStep = patterns[pattern][step];

    if (millis() - previousMillis < currentPatternStep.duration)
        return;

    if (step == 0)
        Log::log("instant pattern start");

    previousMillis = millis();

    for (uint8_t i = 0; i < LED_COUNT; i++)
    {
        uint8_t brightness = currentPatternStep.leds[i];
        DUTY_TYPE gammaValue = PGM_READ_DUTY(&gammaMap[brightness]);
        ledcWrite(ledPins[i], gammaValue);
    }

    PatternManager::nextStep();
}

void InstantPattern::reset()
{
    previousMillis = millis();
}
