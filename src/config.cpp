#include "main.h"

// -- Pin Configuration ---
const int ledPins[LED_COUNT] = {0, 1};
const int buttonPin = 2;

// --- Patterns ---

#define MAX_PATTERN_LENGTH 6
const Step patterns[][MAX_PATTERN_LENGTH] = {
    // Pattern 0: Alternate blink (no fade)
    {{{255, 0}, 300, 0}, {{0, 255}, 300, 0}},
    // Pattern 1: Both fade together
    {{{0, 0}, 800, 1}, {{255, 255}, 800, 1}, {{0, 0}, 800, 1}},
    // Pattern 2: Mixed — blink + fade
    {{{255, 0}, 200, 0}, {{0, 255}, 200, 1}, {{255, 255}, 400, 1}, {{0, 0}, 200, 0}},
};
