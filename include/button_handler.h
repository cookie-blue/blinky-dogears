#ifndef BUTTON_HANDLER_H
#define BUTTON_HANDLER_H

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

#endif
