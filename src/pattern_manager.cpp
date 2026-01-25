#include "pattern_manager.h"

#define NUM_PATTERNS sizeof(patterns) / sizeof(patterns[0])

bool PatternManager::poweredOn = true;
uint8_t PatternManager::currentPattern = 0;
uint8_t PatternManager::currentStep = 0;
uint8_t PatternManager::numPatterns = NUM_PATTERNS;
uint8_t PatternManager::patternLengths[NUM_PATTERNS];

void PatternManager::init()
{
    _calculatePatternLengths();
}

void PatternManager::runPattern()
{
    if (!poweredOn)
        return;

    if (patterns[currentPattern][currentStep].fade)
        FadePattern::run();
    else
        InstantPattern::run();
}

void PatternManager::off()
{
    Log::log("turning off");
    poweredOn = false;
    InstantPattern::reset();
    FadePattern::reset();
    for (uint8_t i = 0; i < LED_COUNT; i++)
        LED::write(ledPins[i], 0);
}

void PatternManager::on()
{
    if (poweredOn)
    {
        Log::log("already on");
        return;
    }

    Log::log("turning on");
    poweredOn = true;
    InstantPattern::reset();
    FadePattern::reset();
}

void PatternManager::override(uint8_t brightness)
{
    poweredOn = false;
    InstantPattern::reset();
    FadePattern::reset();

    for (uint8_t i = 0; i < LED_COUNT; i++)
        LED::write(ledPins[i], brightness);
}


void PatternManager::prevPattern()
{
    currentPattern = (currentPattern - 1 + numPatterns) % numPatterns;
    _switchPattern();
}

void PatternManager::nextPattern()
{
    currentPattern = (currentPattern + 1) % numPatterns;
    _switchPattern();
}

void PatternManager::setPattern(uint8_t newPattern)
{
    if (newPattern >= numPatterns)
    {
        return;
    }

    currentPattern = newPattern;
    _switchPattern();
}

uint8_t PatternManager::getCurrentPattern()
{
    return currentPattern;
}

uint8_t PatternManager::getCurrentStep()
{
    return currentStep;
}

void PatternManager::nextStep()
{
    currentStep = (currentStep + 1) % patternLengths[currentPattern];
}

void PatternManager::_switchPattern()
{
    Log::log("switching to pattern %d", currentPattern);
    poweredOn = true;
    currentStep = 0;
    InstantPattern::reset();
    FadePattern::reset();
    for (uint8_t i = 0; i < LED_COUNT; i++)
        LED::write(ledPins[i], 0);
}

void PatternManager::_calculatePatternLengths()
{
    for (uint8_t pattern = 0; pattern < numPatterns; pattern++)
    {
        uint8_t len = 0;
        for (uint8_t step = 0; step < MAX_PATTERN_LENGTH; step++)
        {
            if (patterns[pattern][step].duration == 0)
                break;
            len++;
        }
        patternLengths[pattern] = len;
    }
}
