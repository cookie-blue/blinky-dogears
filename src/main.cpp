#include <Arduino.h>

// -- Pin Configuration ---
const int ledPins[] = {0, 1};
const int buttonPin = 2;

const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

struct Step
{
  uint8_t leds[numLeds]; // brightness (0–255)
  unsigned int duration; // milliseconds
  bool fade;             // true = fade, false = instant
};

// --- Patterns ---
// Pattern 0: Alternate blink (no fade)
const Step pattern0[] = {{{255, 0}, 300, 0}, {{0, 255}, 300, 0}, {{0, 0}, 0, 0}, {{0, 0}, 0, 0}, {{0, 0}, 0, 0}, {{0, 0}, 0, 0}};
// Pattern 1: Both fade together
const Step pattern1[] = {{{0, 0}, 800, 1}, {{255, 255}, 800, 1}, {{0, 0}, 800, 1}, {{0, 0}, 0, 0}, {{0, 0}, 0, 0}, {{0, 0}, 0, 0}};
// Pattern 2: Mixed — blink + fade
const Step pattern2[] = {{{255, 0}, 200, 0}, {{0, 255}, 200, 1}, {{255, 255}, 400, 1}, {{0, 0}, 200, 0}, {{0, 0}, 0, 0}, {{0, 0}, 0, 0}};

const Step *patterns[] = {pattern0, pattern1, pattern2};

const int numPatterns = sizeof(patterns) / sizeof(patterns[0]);
bool buttonPressed = false;


void setup()
{
  for (int i = 0; i < numLeds; i++)
  {
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(buttonPin, INPUT_PULLUP);
}

int pattern = 0;
int step = 0;
unsigned long previousMillis = 0;
unsigned long fadeStartMillis = 0;
bool fading = false;

int currentBrightness[2] = {0, 0};
int startBrightness[2] = {0, 0};
int targetBrightness[2] = {0, 0};

void checkButton()
{
  if (digitalRead(buttonPin) == LOW && !buttonPressed)
  {
    buttonPressed = true;
    pattern = (pattern + 1) % numPatterns;
    step = 0;
    previousMillis = millis();
  }
  else if (digitalRead(buttonPin) == HIGH)
  {
    buttonPressed = false;
  }
}

void runFadePattern(unsigned long currentMillis, Step currentPatternStep, int length)
{
  int duration = currentPatternStep.duration;

  if (!fading)
  {
    fading = true;
    fadeStartMillis = currentMillis;

    for (int i = 0; i < numLeds; i++)
    {
      startBrightness[i] = currentBrightness[i];
      targetBrightness[i] = currentPatternStep.leds[i];
    }
  }

  float progress = float(currentMillis - fadeStartMillis) / duration;

  for (int i = 0; i < numLeds; i++)
  {
    currentBrightness[i] = startBrightness[i] + (targetBrightness[i] - startBrightness[i]) * min(progress, 1.0);
    analogWrite(ledPins[i], currentBrightness[i]);
  }

  if (progress >= 1.0)
  {
    fading = false;
    previousMillis = currentMillis;
    step = (step + 1) % length;
  }
}

void runInstantPattern(unsigned long currentMillis, Step currentPatternStep, int length)
{
  previousMillis = currentMillis;

  for (int i = 0; i < numLeds; i++)
  {
    analogWrite(ledPins[i], currentPatternStep.leds[i]);
  }

  step = (step + 1) % length;
}

void runPattern()
{
  unsigned long currentMillis = millis();
  Step currentPatternStep = patterns[pattern][step];
  int duration = currentPatternStep.duration;
  bool fade = currentPatternStep.duration;
  int patternLength = sizeof(patterns[pattern]) / sizeof(currentPatternStep);

  if (fade)
  {
    runFadePattern(currentMillis, currentPatternStep, patternLength);
  }
  else if (currentMillis - previousMillis >= duration)
  {
    runInstantPattern(currentMillis, currentPatternStep, patternLength);
  }
}

void loop()
{
  checkButton();
  runPattern();
}
