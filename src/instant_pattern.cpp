#include "instant_pattern.h"
#include "pattern_manager.h"
#include "config.cpp"

unsigned long InstantPattern::previousMillis = 0;

void InstantPattern::run()
{
    int pattern = PatternManager::getCurrentPattern();
    int step = PatternManager::getCurrentStep();
    const Step currentPatternStep = patterns[pattern][step];

    if (millis() - previousMillis < currentPatternStep.duration)
        return;

    if (step == 0)
        log("instant pattern start");

    previousMillis = millis();

    for (int i = 0; i < LED_COUNT; i++)
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
