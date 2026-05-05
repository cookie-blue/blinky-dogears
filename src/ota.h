#pragma once

#include "config.h"
#include "log.h"
#include "indicator.h"
#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <ArduinoOTA.h>

class OTA
{
private:
    static bool _isReady;
    static unsigned long _previousMillis;

public:
    static void init();
    static void handle();
    static bool isReady();
};
