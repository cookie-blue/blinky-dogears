#pragma once

#include "indicator.h"
#include "pattern_manager.h"
#include "log.h"

class ButtonHandler
{
private:
  static bool _buttonPressed;
  static unsigned long _lastDebounceTime;
  static unsigned long _pressStartTime;
  static bool _longPressHandled;

public:
  static void check();
};
