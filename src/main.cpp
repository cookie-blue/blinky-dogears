#include "config.cpp"

#define PWM_FREQUENCY 5000
#define PWM_RESOLUTION 12

const int numPatterns = sizeof(patterns) / sizeof(patterns[0]);
int patternLengths[numPatterns];

// Gamma brightness lookup table <https://victornpb.github.io/gamma-table-generator>
// gamma = 2.2 steps = 256 range = 0-4095 (12 bit)
const uint16_t PROGMEM gammaMap[] = {
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

void setupPins()
{
  Serial.println("setting up pins...");
  ledcSetup(0, PWM_FREQUENCY, PWM_RESOLUTION);
  for (int i = 0; i < LED_COUNT; i++)
  {
    ledcAttachPin(ledPins[i], 0);
  }
  pinMode(buttonPin, INPUT_PULLUP);
}

void calculatePatternLengths()
{
  Serial.println("calculating pattern lengths");
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
  Serial.begin(115200);
  setupPins();
  calculatePatternLengths();
  Serial.println("ready.");
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

  uint16_t gammaValue = pgm_read_word(&gammaMap[brightness]);
  ledcWrite(led, gammaValue);
}

void checkButton()
{
  if (digitalRead(buttonPin) == LOW && !buttonPressed)
  {
    buttonPressed = true;
    pattern = (pattern + 1) % numPatterns;
    Serial.printf("switching pattern to %d\n", pattern);
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
        startBrightness[i] + (targetBrightness[i] - startBrightness[i]) * min(progress, 1.0f));
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
