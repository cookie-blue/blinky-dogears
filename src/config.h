#pragma once

#include <Arduino.h>

// -- Pin Configuration ---
#define LED_COUNT 2
const uint8_t ledPins[LED_COUNT] = {0, 1};

#define STATUS_LED_PIN 8  // comment out to disable
#define STATUS_LED_INVERTED true

#define BUTTON_PIN 9
#define BUTTON_DEBOUNCE_DELAY 50
#define BUTTON_LONG_PRESS_DURATION 3000

// --- BLE Configuration ---
#define ENABLE_BLE
// #define BLE_NAME "blinky"
#define BLE_SERVICE_UUID "12345678-1234-1234-1234-1234567890ab"
#define BLE_CHARACTERISTIC_UUID "abcd1234-5678-90ab-cdef-1234567890ab"

// --- OTA WiFi Configuration ---
#define ENABLE_OTA
#define OTA_BOOT_TIMEOUT 3000
// #define OTA_WIFI_SSID "blinky"
#define OTA_WIFI_PASSWORD "blinkySecure"
#define OTA_WIFI_CHANNEL 1
#define OTA_WIFI_AUTH_MODE WIFI_AUTH_WPA3_PSK
#define OTA_WIFI_CIPHER WIFI_CIPHER_TYPE_AES_GMAC256
// #define OTA_WIFI_AUTH_MODE WIFI_AUTH_WPA2_PSK
// #define OTA_WIFI_CIPHER WIFI_CIPHER_TYPE_CCMP
#define MDNS_NAME "blinky"

// --- Patterns ---
#include "step.h"  // dont' worry about it. just leave it here.

#define MAX_PATTERN_LENGTH 12
const Step patterns[][MAX_PATTERN_LENGTH] = {
    // Pattern 1: Alternate blink 50%
    {{{128, 0}, 300, 0}, {{0, 128}, 300, 0}},
    // Pattern 2: Both fade
    {{{0, 0}, 800, 1}, {{128, 128}, 800, 1}},
    // Pattern 3: alternate fade 0-50%
    {{{128, 0}, 800, 1}, {{0, 128}, 800, 1}},
    // Patern 4: RTW
    {
        {{255, 0}, 160, 0}, {{0, 0}, 60, 0}, {{255, 0}, 25, 0}, {{0, 0}, 10, 0}, {{255, 0}, 25, 0}, {{0, 0}, 160, 0},
        {{0, 255}, 160, 0}, {{0, 0}, 60, 0}, {{0, 255}, 25, 0}, {{0, 0}, 10, 0}, {{0, 255}, 25, 0}, {{0, 0}, 160, 0}
    }
};

// --- PWM Configuration ---
#define PWM_FREQUENCY 9784
#define PWM_RESOLUTION 12
#define DUTY_TYPE uint16_t
#define PGM_READ_DUTY(addr) pgm_read_word(addr)

// --- Gamma Correction ---
// Gamma brightness lookup table <https://victornpb.github.io/gamma-table-generator>
// gamma = 2.2 steps = 256 range = 0-4095 (12 bit)
const DUTY_TYPE PROGMEM gammaMap[] =
{
     0,   0,   0,   0,   0,   1,   1,   2,   2,   3,   3,   4,   5,   6,   7,   8,
     9,  11,  12,  14,  15,  17,  19,  21,  23,  25,  27,  29,  32,  34,  37,  40,
    43,  46,  49,  52,  55,  59,  62,  66,  70,  73,  77,  82,  86,  90,  95,  99,
   104, 109, 114, 119, 124, 129, 135, 140, 146, 152, 158, 164, 170, 176, 182, 189,
   196, 202, 209, 216, 224, 231, 238, 246, 254, 261, 269, 277, 286, 294, 302, 311,
   320, 328, 337, 347, 356, 365, 375, 384, 394, 404, 414, 424, 435, 445, 456, 467,
   477, 488, 500, 511, 522, 534, 545, 557, 569, 581, 594, 606, 619, 631, 644, 657,
   670, 683, 697, 710, 724, 738, 752, 766, 780, 794, 809, 823, 838, 853, 868, 884,
   899, 914, 930, 946, 962, 978, 994,1011,1027,1044,1061,1078,1095,1112,1130,1147,
  1165,1183,1201,1219,1237,1256,1274,1293,1312,1331,1350,1370,1389,1409,1429,1449,
  1469,1489,1509,1530,1551,1572,1593,1614,1635,1657,1678,1700,1722,1744,1766,1789,
  1811,1834,1857,1880,1903,1926,1950,1974,1997,2021,2045,2070,2094,2119,2143,2168,
  2193,2219,2244,2270,2295,2321,2347,2373,2400,2426,2453,2479,2506,2534,2561,2588,
  2616,2644,2671,2700,2728,2756,2785,2813,2842,2871,2900,2930,2959,2989,3019,3049,
  3079,3109,3140,3170,3201,3232,3263,3295,3326,3358,3390,3421,3454,3486,3518,3551,
  3584,3617,3650,3683,3716,3750,3784,3818,3852,3886,3920,3955,3990,4025,4060,4095,
};
