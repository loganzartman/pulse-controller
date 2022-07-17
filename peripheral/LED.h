#include <FastLED.h>

#define LED_COUNT 12
#define PIN_LED 9

#define MODULE_COUNT 6

CRGB leds[LED_COUNT];
double brightness[MODULE_COUNT];
double targetBrightness[MODULE_COUNT];

int pos = 0;
unsigned long lastTime;
unsigned long lastUpdateTime;

// 0x54d9d3

void setupLeds() {
  for (int i = 0; i < MODULE_COUNT; ++i) {
    brightness[i] = 0;
    targetBrightness[i] = 0;
  }
  
  FastLED.addLeds<NEOPIXEL, PIN_LED>(leds, LED_COUNT)
    .setCorrection(CRGB(255, 150, 100));
  lastTime = millis();
  lastUpdateTime = micros();
}

void updateLeds() {
  // simulation
  if (millis() - lastTime > 2000) {
    lastTime = millis();
    pos = (pos + 1) % MODULE_COUNT;
  }
  for (int i = pos; i < MODULE_COUNT; ++i) {
    targetBrightness[i] = 0.0;
  }
  targetBrightness[pos] = 1.0;

  double delta = (micros() - lastUpdateTime) / 1000000.0;
  lastUpdateTime = micros();

  double animSpeed = 1.0 * delta;
  for (int i = 0; i < MODULE_COUNT; ++i) {
    brightness[i] = brightness[i] * (1.0 - animSpeed) + targetBrightness[i] * animSpeed;
  }

  double t = millis()/1000.0;

  FastLED.clear();
  for (int i = 0; i < MODULE_COUNT; ++i) {
    double b = max(0.1, brightness[i]);
    leds[i] = HeatColor(b * 0.75 * 255);
  }
  FastLED.show();
}
