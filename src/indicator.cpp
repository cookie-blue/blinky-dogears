#include "indicator.h"

unsigned long Indicator::_previousMillis = 0;
bool Indicator::_isRunning = false;
bool Indicator::_isOn = false;
uint8_t Indicator::_count;
unsigned int Indicator::_durationOn;
unsigned int Indicator::_durationOff;
uint8_t Indicator::_color[3];
rmt_data_t Indicator::_ledData[24];

#ifdef STATUS_LED_MODE_RGB
void Indicator::buildLedData(bool isOn, const uint8_t color[3])
{
    uint8_t packet[3] = {0, 0, 0};
    if (isOn)
        memcpy(packet, color, sizeof(packet));

    const uint32_t oneHigh = 8;
    const uint32_t oneLow = 4;
    const uint32_t zeroHigh = 4;
    const uint32_t zeroLow = 8;

    int symbol = 0;
    for (int byte = 0; byte < 3; ++byte)
    {
        uint8_t value = packet[byte];
        for (int bit = 7; bit >= 0; --bit)
        {
            bool bitValue = (value >> bit) & 0x01;
            Indicator::_ledData[symbol].level0 = 1;
            Indicator::_ledData[symbol].duration0 = bitValue ? oneHigh : zeroHigh;
            Indicator::_ledData[symbol].level1 = 0;
            Indicator::_ledData[symbol].duration1 = bitValue ? oneLow : zeroLow;
            ++symbol;
        }
    }
}
#endif

void Indicator::init()
{
#ifdef STATUS_LED_PIN
#ifdef STATUS_LED_MODE_RGB
    rmtInit(STATUS_LED_PIN, RMT_TX_MODE, RMT_MEM_NUM_BLOCKS_1, 10000000);
    Log::log("Indicator RGB LED on pin %d is now set", STATUS_LED_PIN);
#else
    pinMode(STATUS_LED_PIN, OUTPUT);
#ifdef STATUS_LED_INVERTED
    digitalWrite(STATUS_LED_PIN, (STATUS_LED_INVERTED ? HIGH : LOW));
    Log::log("Indicator LED on pin %d is now %d", STATUS_LED_PIN, (STATUS_LED_INVERTED ? HIGH : LOW));
#else
    digitalWrite(STATUS_LED_PIN, HIGH);
    Log::log("Indicator LED on pin %d is now %d", STATUS_LED_PIN, HIGH);
#endif
#endif
#else
    Log::log("Indicator LED is disabled in Configuration");
#endif
}

void Indicator::blink(uint8_t count, unsigned int duration, uint8_t color[])
{
    blink(count, duration, 0, color);
}

void Indicator::blink(uint8_t count, unsigned int durationOn, unsigned int durationOff, uint8_t color[])
{
#ifdef STATUS_LED_PIN
    _count = count;
    _durationOn = durationOn;
    _durationOff = durationOff;
    _isRunning = true;
    _isOn = false;
#ifdef STATUS_LED_MODE_RGB
    memcpy(_color, color, sizeof(_color));
#endif
    _previousMillis = millis();
#ifdef STATUS_LED_MODE_RGB
    Log::log("blinking indicator %d times (%d ms / %d ms) in color rgb(%d,%d,%d)", _count, _durationOn, _durationOff, color[0], color[1], color[2]);
#else
    Log::log("blinking indicator %d times (%d ms / %d ms)", _count, _durationOn, _durationOff);
#endif
#endif
}

void Indicator::off()
{
    Indicator::set(LOW);
}

void Indicator::set(uint8_t state)
{
#ifdef STATUS_LED_PIN
    Indicator::_isOn = state;
#ifdef STATUS_LED_MODE_RGB
    Indicator::set(state, _color);
#else
#ifdef STATUS_LED_INVERTED
    state = (STATUS_LED_INVERTED ? !state : state);
#endif
    digitalWrite(STATUS_LED_PIN, state);
#endif
#endif
}

void Indicator::set(uint8_t state, uint8_t color[])
{
#ifdef STATUS_LED_PIN
#ifdef STATUS_LED_MODE_RGB
    Indicator::_isOn = state;
    memcpy(_color, color, sizeof(_color));
    buildLedData(state == HIGH, color);
    rmtWrite(STATUS_LED_PIN, Indicator::_ledData, 24, RMT_WAIT_FOR_EVER);
#else
    (void)color;
    Indicator::set(state);
#endif
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

    Indicator::set(!_isOn);

    _previousMillis = millis();

    if (_count == 0)
    {
        _isRunning = false;
        Log::log("indicator blink done");
    }
#endif
}
