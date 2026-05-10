#include "static_output.h"

uint8_t StaticOutput::pinStates[STATIC_OUTPUT_COUNT];

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
        uint8_t state = STATIC_OUTPUT_INVERTED ? HIGH : LOW;
        digitalWrite(staticOutputPins[i], state);
        StaticOutput::pinStates[i] = state;
    }
}

void StaticOutput::toggle(uint8_t output)
{
    uint8_t state = StaticOutput::pinStates[output];
    digitalWrite(staticOutputPins[output], !state);
    StaticOutput::pinStates[output] = !state;
    Log::log("Static output %d set to %s", output + 1, !state ? "ON" : "OFF");
}
