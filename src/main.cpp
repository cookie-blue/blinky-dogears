#include "config.cpp"

const int numPatterns = sizeof(patterns) / sizeof(patterns[0]);
int patternLengths[numPatterns];
const uint16_t PROGMEM gammaMap[] = {
    0, 0, 0, 0, 1, 1, 2, 3, 4, 6, 8, 10, 13, 16, 19, 24,
    28, 33, 39, 46, 53, 60, 69, 78, 88, 98, 110, 122, 135, 149, 164, 179,
    196, 214, 232, 252, 273, 295, 317, 341, 366, 393, 420, 449, 478, 510, 542, 575,
    610, 647, 684, 723, 764, 806, 849, 894, 940, 988, 1037, 1088, 1140, 1194, 1250, 1307,
    1366, 1427, 1489, 1553, 1619, 1686, 1756, 1827, 1900, 1975, 2051, 2130, 2210, 2293, 2377, 2463,
    2552, 2642, 2734, 2829, 2925, 3024, 3124, 3227, 3332, 3439, 3548, 3660, 3774, 3890, 4008, 4128,
    4251, 4376, 4504, 4634, 4766, 4901, 5038, 5177, 5319, 5464, 5611, 5760, 5912, 6067, 6224, 6384,
    6546, 6711, 6879, 7049, 7222, 7397, 7576, 7757, 7941, 8128, 8317, 8509, 8704, 8902, 9103, 9307,
    9514, 9723, 9936, 10151, 10370, 10591, 10816, 11043, 11274, 11507, 11744, 11984, 12227, 12473, 12722, 12975,
    13230, 13489, 13751, 14017, 14285, 14557, 14833, 15111, 15393, 15678, 15967, 16259, 16554, 16853, 17155, 17461,
    17770, 18083, 18399, 18719, 19042, 19369, 19700, 20034, 20372, 20713, 21058, 21407, 21759, 22115, 22475, 22838,
    23206, 23577, 23952, 24330, 24713, 25099, 25489, 25884, 26282, 26683, 27089, 27499, 27913, 28330, 28752, 29178,
    29608, 30041, 30479, 30921, 31367, 31818, 32272, 32730, 33193, 33660, 34131, 34606, 35085, 35569, 36057, 36549,
    37046, 37547, 38052, 38561, 39075, 39593, 40116, 40643, 41175, 41711, 42251, 42796, 43346, 43899, 44458, 45021,
    45588, 46161, 46737, 47319, 47905, 48495, 49091, 49691, 50295, 50905, 51519, 52138, 52761, 53390, 54023, 54661,
    55303, 55951, 56604, 57261, 57923, 58590, 59262, 59939, 60621, 61308, 62000, 62697, 63399, 64106, 64818, 65535};

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

uint8_t currentBrightness[2] = {0, 0};
uint8_t startBrightness[2] = {0, 0};
uint8_t targetBrightness[2] = {0, 0};

void analogWrite12(uint8_t pin, int value)
{
  cli();

  if (ledPins[0])      Dac[0] = value; // OC1A
  else if (ledPins[1]) Dac[1] = value; // OC1B

  sei();
}

void ledBrightness(uint8_t led, uint8_t brightness)
{
  currentBrightness[led] = brightness;
  analogWrite12(ledPins[led], pgm_read_word(&gammaMap[brightness / 4]));
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
      ledBrightness(ledPins[i], 0);
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
    ledBrightness(
        ledPins[i],
        startBrightness[i] + (targetBrightness[i] - startBrightness[i]) * min(progress, 1.0));
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
    ledBrightness(ledPins[i], currentPatternStep.leds[i]);
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
