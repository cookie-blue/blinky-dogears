#include "led.h"

uint8_t LED::_brightness[LED_COUNT] = {0};

uint8_t LED::_getPinIndex(uint8_t ledPin)
{
    for (uint8_t i = 0; i < LED_COUNT; i++)
    {
        if (ledPins[i] == ledPin)
            return i;
    }

    return 0;
}

void LED::write(uint8_t ledPin, uint8_t brightness)
{
    LED::_brightness[LED::_getPinIndex(ledPin)] = brightness;

    uint8_t pattern = PatternManager::getCurrentPattern();
    float patternBrightness = brightness * patterns[pattern].brightness / 255;

    if (PatternManager::brightnessOverride > 0) {
        patternBrightness = brightness * PatternManager::brightnessOverride / 255;
    }

    DUTY_TYPE gammaValue = PGM_READ_DUTY(&gammaMap[(uint8_t)patternBrightness]);
    ledcWrite(ledPin, gammaValue);
}

uint8_t LED::getBrightness(uint8_t ledPin)
{
    return LED::_brightness[LED::_getPinIndex(ledPin)];
}
