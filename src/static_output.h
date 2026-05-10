#pragma once

#include "config.h"
#include "log.h"

class StaticOutput
{
private:
    static uint8_t pinStates[];
public:
    static void init();
    static void off();
    static void toggle(uint8_t output);
};
