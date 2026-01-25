#pragma once

#include "config.h"
#include "log.h"
#include "led.h"
#include "pattern_manager.h"
#include <cstdarg>

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
