#include "config.cpp"

const int numPatterns = sizeof(patterns) / sizeof(patterns[0]);
int patternLengths[numPatterns];

void setupPins()
{
  for (int i = 0; i < LED_COUNT; i++)
  {
    pinMode(ledPins[i], OUTPUT);
  }
  pinMode(buttonPin, INPUT_PULLUP);
}

void calculatePatternLengths()
{
  for (int p = 0; p < numPatterns; p++)
  {
    int len = 0;
    for (int s = 0; s < MAX_PATTERN_LENGTH; s++)
    {
      if (patterns[p][s].duration == 0)
        break;
      len++;
    }
    patternLengths[p] = len;
  }
}

void setup()
{
  setupPins();
  calculatePatternLengths();
}

int pattern = 0;
int step = 0;
unsigned long previousMillis = 0;
unsigned long fadeStartMillis = 0;
bool fading = false;
bool buttonPressed = false;

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

void runFadePattern(unsigned long currentMillis, Step currentPatternStep)
{
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

  float progress = float(currentMillis - fadeStartMillis) / currentPatternStep.duration;

  for (int i = 0; i < LED_COUNT; i++)
  {
    currentBrightness[i] = startBrightness[i] + (targetBrightness[i] - startBrightness[i]) * min(progress, 1.0);
    analogWrite(ledPins[i], currentBrightness[i]);
  }

  if (progress >= 1.0)
  {
    fading = false;
    previousMillis = currentMillis;
    step = (step + 1) % patternLengths[pattern];
  }
}

void runInstantPattern(unsigned long currentMillis, Step currentPatternStep)
{
  previousMillis = currentMillis;

  for (int i = 0; i < LED_COUNT; i++)
  {
    currentBrightness[i] = currentPatternStep.leds[i];
    analogWrite(ledPins[i], currentPatternStep.leds[i]);
  }

  step = (step + 1) % patternLengths[pattern];
}

void runPattern()
{
  unsigned long currentMillis = millis();
  Step currentPatternStep = patterns[pattern][step];

  if (currentPatternStep.fade)
  {
    runFadePattern(currentMillis, currentPatternStep);
  }
  else if (currentMillis - previousMillis >= currentPatternStep.duration)
  {
    runInstantPattern(currentMillis, currentPatternStep);
  }
}

void loop()
{
  checkButton();
  runPattern();
}
