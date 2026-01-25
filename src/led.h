#pragma once

#include "config.h"

class LED
{
public:
    static void write(uint8_t ledPin, uint8_t brightness);
};
