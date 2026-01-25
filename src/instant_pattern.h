#pragma once

#include "config.h"
#include "log.h"
#include "pattern_manager.h"

class InstantPattern
{
private:
  static unsigned long previousMillis;

public:
  static void run();
  static void reset();
};
