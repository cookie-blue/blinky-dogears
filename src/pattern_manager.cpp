#include "pattern_manager.h"

#define NUM_PATTERNS sizeof(patterns) / sizeof(patterns[0])

bool PatternManager::_poweredOn = true;
uint8_t PatternManager::_currentPattern = 0;
uint8_t PatternManager::_currentStep = 0;
uint8_t PatternManager::_numPatterns = NUM_PATTERNS;
uint8_t PatternManager::_patternLengths[NUM_PATTERNS];

void PatternManager::init()
{
    _calculatePatternLengths();
}

void PatternManager::runPattern()
{
    if (!PatternManager::_poweredOn)
        return;

    if (patterns[PatternManager::_currentPattern][PatternManager::_currentStep].fade)
        FadePattern::run();
    else
        InstantPattern::run();
}

void PatternManager::off()
{
    Log::log("turning off");
    PatternManager::_poweredOn = false;
    InstantPattern::reset();
    FadePattern::reset();
    for (uint8_t i = 0; i < LED_COUNT; i++)
        LED::write(ledPins[i], 0);
}

void PatternManager::on()
{
    if (PatternManager::_poweredOn)
    {
        Log::log("already on");
        return;
    }

    Log::log("turning on");
    PatternManager::_poweredOn = true;
    InstantPattern::reset();
    FadePattern::reset();
}

void PatternManager::override(uint8_t brightness)
{
    PatternManager::_poweredOn = false;
    InstantPattern::reset();
    FadePattern::reset();

    for (uint8_t i = 0; i < LED_COUNT; i++)
        LED::write(ledPins[i], brightness);
}

void PatternManager::prevPattern()
{
    PatternManager::_currentPattern = (PatternManager::_currentPattern - 1 + PatternManager::_numPatterns) % PatternManager::_numPatterns;
    PatternManager::_switchPattern();
}

void PatternManager::nextPattern()
{
    PatternManager::_currentPattern = (PatternManager::_currentPattern + 1) % PatternManager::_numPatterns;
    PatternManager::_switchPattern();
}

void PatternManager::setPattern(uint8_t newPattern)
{
    if (newPattern >= PatternManager::_numPatterns)
    {
        return;
    }

    PatternManager::_currentPattern = newPattern;
    PatternManager::_switchPattern();
}

uint8_t PatternManager::getCurrentPattern()
{
    return PatternManager::_currentPattern;
}

uint8_t PatternManager::getCurrentStep()
{
    return PatternManager::_currentStep;
}

void PatternManager::nextStep()
{
    PatternManager::_currentStep = (PatternManager::_currentStep + 1) % PatternManager::_patternLengths[PatternManager::_currentPattern];
}

void PatternManager::_switchPattern()
{
    Log::log("switching to pattern %d", PatternManager::_currentPattern);
    PatternManager::_poweredOn = true;
    PatternManager::_currentStep = 0;
    InstantPattern::reset();
    FadePattern::reset();
    for (uint8_t i = 0; i < LED_COUNT; i++)
        LED::write(ledPins[i], 0);
}

void PatternManager::_calculatePatternLengths()
{
    for (uint8_t pattern = 0; pattern < PatternManager::_numPatterns; pattern++)
    {
        uint8_t len = 0;
        for (uint8_t step = 0; step < MAX_PATTERN_LENGTH; step++)
        {
            if (patterns[pattern][step].duration == 0)
                break;
            len++;
        }
        PatternManager::_patternLengths[pattern] = len;
    }
}
