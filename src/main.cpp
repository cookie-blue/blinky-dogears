#include "config.cpp"

const int numPatterns = sizeof(patterns) / sizeof(patterns[0]);
int patternLengths[numPatterns];

#ifdef PWM_12_BIT
const uint16_t PROGMEM gammaMap[] = {
     0,   0,   0,   0,   0,   1,   1,   2,   2,   3,   3,   4,   5,   6,   7,   8,
     9,  11,  12,  14,  15,  17,  19,  21,  23,  25,  27,  29,  32,  34,  37,  40,
    43,  46,  49,  52,  55,  59,  62,  66,  70,  73,  77,  82,  86,  90,  95,  99,
   104, 109, 114, 119, 124, 129, 135, 140, 146, 152, 158, 164, 170, 176, 182, 189,
   196, 202, 209, 216, 224, 231, 238, 246, 254, 261, 269, 277, 286, 294, 302, 311,
   320, 329, 338, 347, 356, 365, 375, 385, 394, 404, 414, 424, 435, 445, 456, 467,
   477, 489, 500, 511, 522, 534, 546, 557, 569, 582, 594, 606, 619, 631, 644, 657,
   670, 684, 697, 710, 724, 738, 752, 766, 780, 795, 809, 824, 838, 853, 869, 884,
   899, 915, 930, 946, 962, 978, 994,1011,1027,1044,1061,1078,1095,1112,1130,1147,
  1165,1183,1201,1219,1238,1256,1275,1293,1312,1331,1351,1370,1389,1409,1429,1449,
  1469,1489,1510,1530,1551,1572,1593,1614,1636,1657,1679,1700,1722,1745,1767,1789,
  1812,1834,1857,1880,1904,1927,1950,1974,1998,2022,2046,2070,2095,2119,2144,2169,
  2194,2219,2245,2270,2296,2322,2348,2374,2400,2427,2453,2480,2507,2534,2561,2589,
  2616,2644,2672,2700,2728,2757,2785,2814,2843,2872,2901,2931,2960,2990,3020,3050,
  3080,3110,3141,3171,3202,3233,3264,3295,3327,3359,3390,3422,3454,3487,3519,3552,
  3585,3618,3651,3684,3717,3751,3785,3819,3853,3887,3921,3956,3991,4026,4061,4096,
};
#else
const uint8_t PROGMEM gammaMap[] = {
     0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   1,
     1,   1,   1,   1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,
     3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,   6,   6,   6,
     6,   7,   7,   7,   8,   8,   8,   9,   9,   9,  10,  10,  11,  11,  11,  12,
    12,  13,  13,  13,  14,  14,  15,  15,  16,  16,  17,  17,  18,  18,  19,  19,
    20,  20,  21,  22,  22,  23,  23,  24,  25,  25,  26,  26,  27,  28,  28,  29,
    30,  30,  31,  32,  33,  33,  34,  35,  35,  36,  37,  38,  39,  39,  40,  41,
    42,  43,  43,  44,  45,  46,  47,  48,  49,  49,  50,  51,  52,  53,  54,  55,
    56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,
    73,  74,  75,  76,  77,  78,  79,  81,  82,  83,  84,  85,  87,  88,  89,  90,
    91,  93,  94,  95,  97,  98,  99, 100, 102, 103, 105, 106, 107, 109, 110, 111,
   113, 114, 116, 117, 119, 120, 121, 123, 124, 126, 127, 129, 130, 132, 133, 135,
   137, 138, 140, 141, 143, 145, 146, 148, 149, 151, 153, 154, 156, 158, 159, 161,
   163, 165, 166, 168, 170, 172, 173, 175, 177, 179, 181, 182, 184, 186, 188, 190,
   192, 194, 196, 197, 199, 201, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221,
   223, 225, 227, 229, 231, 234, 236, 238, 240, 242, 244, 246, 248, 251, 253, 255,
};
#endif

#ifdef PWM_12_BIT
// 12-bit PWM magic
volatile uint16_t Dac[] = {0, 0};
volatile uint8_t Cycle = 0;
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

  Cycle = (Cycle + 1) & 0x0F;
}

void setupPWM()
{
  // Top value for high (Table 12-2)
  OCR1C = 255;

  // Enable both PWM channels (A & B)
  TCCR1 = (1 << PWM1A) | (1 << COM1A1) |
          (1 << PWM1B) | (1 << COM1B1) |
          (1 << CS10);

  TIMSK |= 1 << TOIE1; // Timer/Counter1 Overflow Interrupt Enable
}
#endif

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
  #ifdef PWM_12_BIT
  setupPWM();
  #endif
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

void ledBrightness(uint8_t led, uint8_t brightness)
{
  currentBrightness[led] = brightness;

  #ifdef PWM_12_BIT
  cli();
  Dac[led] = pgm_read_word(&gammaMap[brightness]);
  sei();
  #else
  analogWrite(ledPins[led], pgm_read_word_near(&gammaMap[brightness]));
  #endif
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
      ledBrightness(i, 0);
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
        i,
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
    ledBrightness(i, currentPatternStep.leds[i]);
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
