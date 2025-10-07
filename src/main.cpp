#include "config.cpp"

const int numPatterns = sizeof(patterns) / sizeof(patterns[0]);
bool buttonPressed = false;

void setup()
{
  for (int i = 0; i < LED_COUNT; i++)
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

    for (int i = 0; i < LED_COUNT; i++)
    {
      startBrightness[i] = currentBrightness[i];
      targetBrightness[i] = currentPatternStep.leds[i];
    }
  }

  float progress = float(currentMillis - fadeStartMillis) / duration;

  for (int i = 0; i < LED_COUNT; i++)
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

  for (int i = 0; i < LED_COUNT; i++)
  {
    currentBrightness[i] = currentPatternStep.leds[i];
    analogWrite(ledPins[i], currentPatternStep.leds[i]);
  }

  step = (step + 1) % length;
}

void runPattern()
{
  unsigned long currentMillis = millis();
  Step currentPatternStep = patterns[pattern][step];
  int duration = currentPatternStep.duration;
  bool fade = currentPatternStep.fade;
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
