#include "main.h"

String get_id()
{
    String id;
    uint64_t chipid;
    char ssid[13];
    chipid = ESP.getEfuseMac();
    uint16_t chip = (uint16_t)(chipid >> 32);
    snprintf(ssid, 13, "%04X%08X", chip, (uint32_t)chipid);
    for (int i = 0; i < 12; i++)
    {
        id += String(ssid[i]);
    }
    return String(id);
}

void setup()
{
    Serial.begin(115200);

    delay(1000);

    Log::log("setting up pins...");
    for (uint8_t i = 0; i < LED_COUNT; i++)
        ledcAttach(ledPins[i], PWM_FREQUENCY, PWM_RESOLUTION);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    Indicator::init();

    PatternManager::init();
    BLE::init();

    Log::log("ready.");
}

void loop()
{
    ButtonHandler::check();
    PatternManager::runPattern();
    Indicator::run();
}
