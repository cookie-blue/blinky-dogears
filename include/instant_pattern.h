#ifndef INSTANT_PATTERN_H
#define INSTANT_PATTERN_H

#include "main.h"

class InstantPattern
{
private:
  static unsigned long previousMillis;

public:
  static void run();
  static void reset();
};

#endif
