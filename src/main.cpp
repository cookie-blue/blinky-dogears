#include "config.cpp"
#include "fade_pattern.h"
#include "instant_pattern.h"
#include "button_handler.h"
#include "pattern_manager.h"
#include <cstdarg>

void setup()
{
    Serial.begin(115200);

    log("setting up pins...");
    for (int i = 0; i < LED_COUNT; i++)
    ledcAttach(ledPins[i], PWM_FREQUENCY, PWM_RESOLUTION);

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    #if STATUS_LED_PIN >= 0
    pinMode(STATUS_LED_PIN, OUTPUT);
    digitalWrite(STATUS_LED_PIN, HIGH);
    #endif

    PatternManager::init();
    log("ready.");
}

void loop()
{
    ButtonHandler::check();
    PatternManager::runPattern();
}

void log(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    uint8_t pattern = PatternManager::getCurrentPattern();
    float time = millis() / 1000.0;

    Serial.printf("[%6.1f][%d] ", time, pattern);
    Serial.vprintf(format, args);
    Serial.println();

    va_end(args);
}
