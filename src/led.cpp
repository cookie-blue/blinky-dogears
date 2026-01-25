#include "led.h"

void LED::write(uint8_t ledPin, uint8_t brightness)
{
    DUTY_TYPE gammaValue = PGM_READ_DUTY(&gammaMap[brightness]);
    ledcWrite(ledPin, gammaValue);
}
