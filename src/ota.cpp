#include "ota.h"

bool OTA::_isReady = false;
unsigned long OTA::_previousMillis = 0;

void OTA::init()
{
#ifndef ENABLE_OTA
    Log::log("OTA is disabled in Configuration");
    return;
#endif
    Log::log("Starting OTA...");

#ifdef BLE_NAME
    char ssid[] = OTA_WIFI_SSID;
#else
    uint32_t chipid = ESP.getEfuseMac();
    char ssid[7 + 13 + 1];
    snprintf(ssid, (7 + 13 + 1), "Blinky %08X", (uint32_t)chipid);
#endif
    Log::log("OTA SSID: %s", ssid);
    if (!WiFi.softAP(ssid, OTA_WIFI_PASSWORD, OTA_WIFI_CHANNEL, false, 1, false, OTA_WIFI_AUTH_MODE, OTA_WIFI_CIPHER))
    {
        Log::log("Error setting up WiFi AP!");
        delay(1000);
        ESP.restart();
    }
    WiFi.softAPenableIPv6();
    IPAddress ipv4address = WiFi.softAPIP();
    IPAddress ipv6address = WiFi.softAPlinkLocalIPv6();
    Log::log("OTA IPv4 address: %s", ipv4address.toString());
    Log::log("OTA IPv6 address: %s", ipv6address.toString());

    ArduinoOTA.setHostname(MDNS_NAME);
    ArduinoOTA.setPort(3232);
    ArduinoOTA.setMdnsEnabled(true);
    ArduinoOTA.onStart([]()
                       { Log::log("OTA start"); });
    ArduinoOTA.onEnd([]()
                     { Log::log("OTA end"); });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
                          { Log::log("OTA  progress: %u%%", progress / (total / 100)); });
    ArduinoOTA.onError([](ota_error_t error)
                       {
                             if (error == OTA_AUTH_ERROR)
                               Log::log("OTA error: auth");
                             else if (error == OTA_BEGIN_ERROR)
                               Log::log("OTA error: begin");
                             else if (error == OTA_CONNECT_ERROR)
                               Log::log("OTA error: connect");
                             else if (error == OTA_RECEIVE_ERROR)
                               Log::log("OTA error: receive");
                             else if (error == OTA_END_ERROR)
                               Log::log("OTA error: end"); });
    ArduinoOTA.begin();
    Log::log("OTA ready");
    Indicator::blink(1, 100);
    OTA::_isReady = true;
}

void OTA::handle()
{
    ArduinoOTA.handle();
    if (millis() - OTA::_previousMillis >= 2000)
    {
        OTA::_previousMillis = millis();
        Indicator::blink(1, 100);
    }
}

bool OTA::isReady()
{
    return OTA::_isReady;
}
