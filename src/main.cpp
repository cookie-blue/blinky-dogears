#include "config.cpp"

const int numPatterns = sizeof(patterns) / sizeof(patterns[0]);
int patternLengths[numPatterns];
// 12-bit PWM magic
volatile int Dac[] = {0, 0};
volatile int Cycle = 0;
volatile uint8_t activeChannel = 0; // 0 = none, 1 = A, 2 = B

// Overflow interrupt
ISR(TIMER1_OVF_vect)
{
  static int remain[2];
  if (Cycle == 0)
  {
    remain[0] = Dac[0];
    remain[1] = Dac[1];
  }

  // Array of OCR registers
  volatile uint8_t *OCRs[2] = {&OCR1A, &OCR1B};

  for (uint8_t i = 0; i < 2; i++)
  {
    if (remain[i] >= 256)
    {
      *OCRs[i] = 255;
      remain[i] -= 256;
    }
    else
    {
      *OCRs[i] = remain[i];
      remain[i] = 0;
    }
  }

  Cycle = (Cycle + 1) & 0x03;
}

void setupPWM()
{
  // Top value for high (Table 12-2)
  OCR1C = 255;

  // Enable both PWM channels (A & B)
  TCCR1 = (1 << PWM1A) | (1 << COM1A0) |
          (1 << PWM1B) | (1 << COM1B0) |
          (1 << CS10);

  TIMSK |= 1 << TOIE1; // Timer/Counter1 Overflow Interrupt Enable
}

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
  setupPWM();
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

void analogWrite12(uint8_t pin, int value)
{
  cli();

  if (ledPins[0])      Dac[0] = value; // OC1A
  else if (ledPins[1]) Dac[1] = value; // OC1B

  sei();
}


void checkButton()
{
  if (digitalRead(buttonPin) == LOW && !buttonPressed)
  {
    buttonPressed = true;
    pattern = (pattern + 1) % numPatterns;
    step = 0;
    previousMillis = millis();
    for (int i = 0; i < LED_COUNT; i++)
    {
      analogWrite(ledPins[i], 0);
    }
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
