#pragma once

#include "config.h"

class StaticOutput
{
public:
    static void init();
    static void off();
    static void toggle(uint8_t output);
};
