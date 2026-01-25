#ifndef FADE_PATTERN_H
#define FADE_PATTERN_H

#include "main.h"

class FadePattern
{
private:
  static bool fading;
  static unsigned long fadeStartMillis;

  static void _start(const Step &currentPatternStep);

public:
  static void run();
  static void reset();
};

#endif
