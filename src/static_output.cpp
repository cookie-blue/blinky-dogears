#include "static_output.h"

#include "log.h"

void StaticOutput::init()
{
#if STATIC_OUTPUT_COUNT > 0
    Log::log("Setting up static outputs");
    for (uint8_t i = 0; i < STATIC_OUTPUT_COUNT; i++)
    {
        pinMode(staticOutputPins[i], OUTPUT);
    }
    StaticOutput::off();
#else
    Log::log("No static outputs configured");
#endif
}

void StaticOutput::off()
{
    for (uint8_t i = 0; i < STATIC_OUTPUT_COUNT; i++)
    {
        digitalWrite(staticOutputPins[i], STATIC_OUTPUT_INVERTED ? HIGH : LOW);
    }
}

void StaticOutput::toggle(uint8_t output)
{
    uint8_t state = digitalRead(staticOutputPins[output]);
    digitalWrite(staticOutputPins[output], !state);
    state = STATIC_OUTPUT_INVERTED ? !state : state;
    Log::log("Static output %d set to %s", output + 1, !state ? "ON" : "OFF");
}
