#include "log.h"

void Log::log(const char *format, ...)
{
    va_list args;
    va_start(args, format);

    uint8_t pattern = PatternManager::getCurrentPattern();
    float time = millis() / 1000.0;

    Serial.printf("[%6.1f][%d] ", time, pattern);
    Serial.vprintf(format, args);
    Serial.println();

    va_end(args);
};
