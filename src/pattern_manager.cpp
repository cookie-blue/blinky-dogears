#include "pattern_manager.h"
#include "config.cpp"
#include "instant_pattern.h"
#include "fade_pattern.h"
#include <cstdlib>

#define NUM_PATTERNS sizeof(patterns) / sizeof(patterns[0])

int PatternManager::currentPattern = 0;
int PatternManager::currentStep = 0;
int PatternManager::numPatterns = NUM_PATTERNS;
int PatternManager::patternLengths[NUM_PATTERNS];

#define LED_COUNT 2

void PatternManager::init()
{
    _calculatePatternLengths();
}

void PatternManager::runPattern()
{
    if (patterns[currentPattern][currentStep].fade)
        FadePattern::run();
    else
        InstantPattern::run();
}

void PatternManager::nextPattern()
{
    currentPattern = (currentPattern + 1) % numPatterns;
    log("switching to pattern %d", currentPattern);
    currentStep = 0;
    InstantPattern::reset();
    FadePattern::reset();
    for (int i = 0; i < LED_COUNT; i++)
        ledcWrite(ledPins[i], 0);
}

int PatternManager::getCurrentPattern()
{
    return currentPattern;
}

int PatternManager::getCurrentStep()
{
    return currentStep;
}

void PatternManager::nextStep()
{
    currentStep = (currentStep + 1) % patternLengths[currentPattern];
}

void PatternManager::_calculatePatternLengths()
{
    for (int pattern = 0; pattern < numPatterns; pattern++)
    {
        int len = 0;
        for (int step = 0; step < MAX_PATTERN_LENGTH; step++)
        {
            if (patterns[pattern][step].duration == 0)
                break;
            len++;
        }
        patternLengths[pattern] = len;
    }
}
