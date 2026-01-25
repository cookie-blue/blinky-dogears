#pragma once

#include "pattern_manager.h"

class ButtonHandler
{
private:
  static bool buttonPressed;
  static unsigned long lastDebounceTime;
  static unsigned long pressStartTime;
  static bool longPressHandled;

public:
  static void check();
};
