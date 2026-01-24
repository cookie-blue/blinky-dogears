#ifndef PATTERN_MANAGER_H
#define PATTERN_MANAGER_H

#include <stdint.h>

class PatternManager
{
private:
    static int currentPattern;
    static int currentStep;
    static int numPatterns;
    static int patternLengths[];
    static void _calculatePatternLengths();

public:
    static void init();
    static void nextPattern();
    static int getCurrentPattern();
    static int getCurrentStep();
    static void nextStep();
};

#endif
