#include "main.h"

#define GAMMA 2.2

// -- Pin Configuration ---
const int ledPins[LED_COUNT] = {0, 1};
const int buttonPin = 2;

// --- Patterns ---
#define MAX_PATTERN_LENGTH 12
const Step patterns[][MAX_PATTERN_LENGTH] = {
    // Pattern 1: Alternate blink 25%
    {{{32, 0}, 300, 0}, {{0, 32}, 300, 0}},
    // Pattern 2: Both fade
    {{{0, 0}, 800, 1}, {{32, 32}, 800, 1}},
    // Pattern 3: alternate fade 0-25%
    {{{32, 0}, 800, 1}, {{0, 32}, 800, 1}},
    // Patern 4: RTW
    {
        {{255, 0}, 160, 0}, {{0, 0}, 60, 0}, {{255, 0}, 25, 0}, {{0, 0}, 10, 0}, {{255, 0}, 25, 0}, {{0, 0}, 160, 0},
        {{0, 255}, 160, 0}, {{0, 0}, 60, 0}, {{0, 255}, 25, 0}, {{0, 0}, 10, 0}, {{0, 255}, 25, 0}, {{0, 0}, 160, 0}
    }
};
