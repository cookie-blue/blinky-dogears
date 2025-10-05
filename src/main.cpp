#include <Arduino.h>

// -- Pin Configuration ---
const int ledPins[] = {0, 1};
const int buttonPin = 2;

// --- Patterns ---
// patterns[patternIndex][stepIndex][led0, led1, duration_ms]
#define MAX_PATTERN_LENGTH 8
#define LED1 0
#define LED2 1
#define DURATION 2
const int patterns[][MAX_PATTERN_LENGTH][3] = {
    // Pattern 0: Alternate blink
    {{255, 0, 200}, {0, 255, 200}, {-1, -1, 0}, {-1, -1, 0}, {-1, -1, 0}, {-1, -1, 0}, {-1, -1, 0}, {-1, -1, 0}},
    // Pattern 1: Both blink together
    {{255, 255, 400}, {0, 0, 400}, {-1, -1, 0}, {-1, -1, 0}, {-1, -1, 0}, {-1, -1, 0}, {-1, -1, 0}, {-1, -1, 0}},
    // Pattern 2: Chase effect
    {{255, 0, 150}, {0, 255, 150}, {255, 255, 150}, {0, 0, 150}, {-1, -1, 0}, {-1, -1, 0}, {-1, -1, 0}, {-1, -1, 0}},
};

const int numPatterns = sizeof(patterns) / sizeof(patterns[0]);
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);
bool buttonPressed = false;

int getPatternLength(int pattern)
{
  int length = 0;
  for (int i = 0; i < MAX_PATTERN_LENGTH; i++)
  {
    // is empty pattern step
    if (patterns[pattern][i][LED1] < 0 ||
        patterns[pattern][i][LED2] < 0 ||
        patterns[pattern][i][DURATION] <= 0)
    {
      break;
    }
    length++;
  }
  return length;
}

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

void runPattern()
{
  unsigned long currentMillis = millis();
  int length = getPatternLength(pattern);
  int duration = patterns[pattern][step][DURATION];

  if (currentMillis - previousMillis >= duration)
  {
    previousMillis = currentMillis;

    for (int i = 0; i < numLeds; i++)
    {
      analogWrite(ledPins[i], patterns[pattern][step][i]);
    }

    step = (step + 1) % length;
  }
}

void loop()
{
  checkButton();
  runPattern();
}
