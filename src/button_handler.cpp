#include "button_handler.h"
#include "config.cpp"
#include "pattern_manager.h"

bool ButtonHandler::buttonPressed = false;
unsigned long ButtonHandler::lastDebounceTime = 0;
const unsigned long ButtonHandler::DEBOUNCE_DELAY = 50; // milliseconds

void ButtonHandler::check()
{
    int reading = digitalRead(buttonPin);

    if (reading == LOW && !buttonPressed)
    {
        if (millis() - lastDebounceTime > DEBOUNCE_DELAY)
        {
            buttonPressed = true;
            PatternManager::nextPattern();
            lastDebounceTime = millis();
        }
    }
    else if (reading == HIGH)
    {
        if (millis() - lastDebounceTime > DEBOUNCE_DELAY)
        {
            buttonPressed = false;
            lastDebounceTime = millis();
        }
    }
}
