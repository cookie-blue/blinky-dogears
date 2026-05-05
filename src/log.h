#pragma once

#include <cstdarg>
#include "pattern_manager.h"

class Log
{
public:
    static void log(const char *format, ...);
};
