#include "instant_pattern.h"

unsigned long InstantPattern::_previousMillis = 0;

void InstantPattern::run()
{
    uint8_t pattern = PatternManager::getCurrentPattern();
    uint8_t step = PatternManager::getCurrentStep();
    const Step currentPatternStep = patterns[pattern][step];

    if (millis() - InstantPattern::_previousMillis < currentPatternStep.duration)
        return;

    if (step == 0)
        Log::log("instant pattern start");

    InstantPattern::_previousMillis = millis();

    for (uint8_t i = 0; i < LED_COUNT; i++)
    {
        uint8_t brightness = currentPatternStep.leds[i];
        LED::write(ledPins[i], brightness);
    }

    PatternManager::nextStep();
}

void InstantPattern::reset()
{
    InstantPattern::_previousMillis = millis();
}
