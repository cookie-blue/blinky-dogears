#include "indicator.h"

unsigned long Indicator::_previousMillis = 0;
bool Indicator::_isRunning = false;
bool Indicator::_isOn = false;
uint8_t Indicator::_count;
unsigned int Indicator::_durationOn;
unsigned int Indicator::_durationOff;

void Indicator::init()
{
    #ifdef STATUS_LED_PIN
        pinMode(STATUS_LED_PIN, OUTPUT);
        digitalWrite(STATUS_LED_PIN, (STATUS_LED_INVERTED ? HIGH : LOW));
        Log::log("Indicator LED on pin %d is now %d", STATUS_LED_PIN, (STATUS_LED_INVERTED ? HIGH : LOW));
    #endif
    Log::log("Indicator LED is disabled in Configuration");
}

void Indicator::blink(uint8_t count, unsigned int duration)
{
    blink(count, duration, 0);
}

void Indicator::blink(uint8_t count, unsigned int durationOn, unsigned int durationOff)
{
    #ifdef STATUS_LED_PIN
        _count = count;
        _durationOn = durationOn;
        _durationOff = durationOff;
        _isRunning = true;
        _isOn = false;
        _previousMillis = millis();
        Log::log("blinking indicator %d times (%d ms / %d ms)", _count, _durationOn, _durationOff);
    #endif
}

void Indicator::run()
{
    #ifdef STATUS_LED_PIN
        if (!_isRunning)
            return;

        unsigned int duration = (_isOn ? _durationOn : _durationOff);
        if (millis() - _previousMillis < duration)
            return;

        if (_isOn)
            _count--;

        bool newStatus = (STATUS_LED_INVERTED ? _isOn : !_isOn);
        digitalWrite(STATUS_LED_PIN, (newStatus ? HIGH : LOW));
        _isOn = !_isOn;

        _previousMillis = millis();

        if (_count == 0)
        {
            _isRunning = false;
            Log::log("indicator blink done");
        }
    #endif
}
