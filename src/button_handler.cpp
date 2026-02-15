#include "button_handler.h"

bool ButtonHandler::_buttonPressed = false;
unsigned long ButtonHandler::_lastDebounceTime = 0;
unsigned long ButtonHandler::_pressStartTime = 0;
bool ButtonHandler::_longPressHandled = false;

void ButtonHandler::check()
{
    uint8_t reading = digitalRead(BUTTON_PIN);

    if (reading == LOW && !ButtonHandler::_buttonPressed)
    {
        if (millis() - ButtonHandler::_lastDebounceTime > BUTTON_DEBOUNCE_DELAY)
        {
            ButtonHandler::_buttonPressed = true;
            ButtonHandler::_pressStartTime = millis();
            ButtonHandler::_longPressHandled = false;
            PatternManager::nextPattern();
            ButtonHandler::_lastDebounceTime = millis();
        }
    }
    else if (reading == LOW && ButtonHandler::_buttonPressed)
    {
        // Button is still pressed - check for long press
        unsigned long pressDuration = millis() - ButtonHandler::_pressStartTime;
        if (pressDuration >= BUTTON_LONG_PRESS_DURATION && !ButtonHandler::_longPressHandled)
        {
            Log::log("Long press detected: %lu ms", pressDuration);
            Indicator::blink(1, 100);

            _longPressHandled = true;
        }
    }
    else if (reading == HIGH)
    {
        if (millis() - _lastDebounceTime > BUTTON_DEBOUNCE_DELAY)
        {
            ButtonHandler::_buttonPressed = false;
            ButtonHandler::_lastDebounceTime = millis();
        }
    }
}
