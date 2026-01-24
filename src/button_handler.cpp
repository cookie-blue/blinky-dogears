#include "button_handler.h"
#include "config.cpp"
#include "pattern_manager.h"

bool ButtonHandler::buttonPressed = false;
unsigned long ButtonHandler::lastDebounceTime = 0;
unsigned long ButtonHandler::pressStartTime = 0;
bool ButtonHandler::longPressHandled = false;

void ButtonHandler::check()
{
    int reading = digitalRead(BUTTON_PIN);

    if (reading == LOW && !buttonPressed)
    {
        if (millis() - lastDebounceTime > BUTTON_DEBOUNCE_DELAY)
        {
            buttonPressed = true;
            pressStartTime = millis();
            longPressHandled = false;
            PatternManager::nextPattern();
            lastDebounceTime = millis();
        }
    }
    else if (reading == LOW && buttonPressed)
    {
        // Button is still pressed - check for long press
        unsigned long pressDuration = millis() - pressStartTime;
        if (pressDuration >= BUTTON_LONG_PRESS_DURATION && !longPressHandled)
        {
            log("Long press detected: %lu ms", pressDuration);

            #if SATUS_LED_PIN >= 0
                digitalWrite(STATUS_LED_PIN, LOW);
                delay(100);
                digitalWrite(STATUS_LED_PIN, HIGH);
            #endif
            longPressHandled = true;
        }
    }
    else if (reading == HIGH)
    {
        if (millis() - lastDebounceTime > BUTTON_DEBOUNCE_DELAY)
        {
            buttonPressed = false;
            lastDebounceTime = millis();
        }
    }
}
