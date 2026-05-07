#include "led.h"

void LED::write(uint8_t ledPin, uint8_t brightness)
{
    uint8_t pattern = PatternManager::getCurrentPattern();
    float patternBrightness = (patterns[pattern].brightness / 255) * brightness;

    if (PatternManager::brightnessOverride > 0) {
        patternBrightness = (PatternManager::brightnessOverride / 255) * brightness;
    }

    DUTY_TYPE gammaValue = PGM_READ_DUTY(&gammaMap[(uint8_t)patternBrightness]);
    ledcWrite(ledPin, gammaValue);
}
