#include "config.cpp"
#include "fade_pattern.h"
#include "instant_pattern.h"
#include "button_handler.h"
#include "pattern_manager.h"
#include "ble.h"
#include <cstdarg>

String get_id(){
  String id;
  uint64_t chipid;
  char ssid[13];
  chipid = ESP.getEfuseMac();
  uint16_t chip = (uint16_t)(chipid >> 32);
  snprintf(ssid, 13, "%04X%08X", chip, (uint32_t)chipid);
  for ( int i = 0; i < 12; i++){
    id += String(ssid[i]);
  }
  return String(id);
}


void setup()
{
    Serial.begin(115200);

    delay(1000);

    log("setting up pins...");
    for (uint8_t i = 0; i < LED_COUNT; i++)
    ledcAttach(ledPins[i], PWM_FREQUENCY, PWM_RESOLUTION);

    pinMode(BUTTON_PIN, INPUT_PULLUP);

    #ifdef STATUS_LED_PIN
        pinMode(STATUS_LED_PIN, OUTPUT);
        digitalWrite(STATUS_LED_PIN, HIGH);
    #endif

    PatternManager::init();

    #ifdef ENABLE_BLE
        BLE::init();
    #endif

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
