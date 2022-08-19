#include <FastLED.h>
#include "pinout.h"
#include "Stats.h"

#define N_MODULES 5
#define N_LEDS 69 * N_MODULES

CRGB leds[N_LEDS];

CRGB hot_color(float heat, float brightness) {
  float heat3 = heat * heat * heat;
  return CRGB(
    brightness * heat * 255, 
    brightness * heat3 * 255, 
    brightness * heat3 * heat3 * 255
  );
}

CRGB hot_color(float heat, float brightness, uint8_t hue) {
  float heat2 = heat * heat;
  uint8_t i1 = brightness * heat * 255;
  uint8_t i2 = brightness * heat2 * 255;
  uint8_t i3 = brightness * heat2 * heat2 * 255;
  return CHSV(hue, 255, i1) + CHSV(hue + 255 / 3, 255, i2) + CHSV(hue + 255 * 2 / 3, 255, i3);
}

enum AnimationMode {TEST, WHITE, ACTIVE};

struct Animation {
  int module_index;
  CRGB* const leds;
  AnimationMode mode = ACTIVE;
  
  unsigned long clock_prev_ms = 0;
  unsigned long clock_ms = 0;
  float dt_s = 0;
  
  int stage = 0;
  unsigned long stage_start_ms = 0;

  float activation_rate = 0.5;
  float activation_target = 0;
  float activation = 0;

  Animation(int module_index, CRGB* const leds, unsigned long clock_ms) 
    : module_index(module_index),
      leds(leds), 
      clock_prev_ms(clock_ms),
      clock_ms(clock_ms) {}

  void update(unsigned long clock_ms, int stage) {
    this->dt_s = (clock_ms - this->clock_prev_ms) / 1000.0;
    this->clock_prev_ms = this->clock_ms;
    this->clock_ms = clock_ms;
    if (this->stage != stage) {
      this->stage = stage;
      this->stage_start_ms = clock_ms;
    }

    switch (mode) {
      case TEST:
        updateTest();
        break;
      case WHITE:
        updateWhite();
        break;
      case ACTIVE:
        updateActive();
        break;
    }
  }

private:
  void updateTest() {
    static const unsigned long d = 200;
    if (clock_ms / d % N_MODULES == module_index) {
//      fill_solid(leds, N_LEDS, CHSV(module_index * 255 / N_MODULES, 255, 255));
    }
    fill_solid(leds, N_LEDS, CHSV(module_index * 255 / N_MODULES + millis() / 2, 255, 255));
  }

  void updateWhite() {
    fill_solid(leds, N_LEDS, CRGB(255, 255, 255));
  }

  void updateActive() {
    if (module_index < stage) {
      // activated
      activation_target = 1.0;
    } else {
      activation_target = 0.0;
    }
    activation += (activation_target - activation) * activation_rate * dt_s;

    for (int i = 0; i < N_LEDS; ++i) {
      float heat = inoise8(clock_ms / 4, module_index * 67 + clock_ms / 3, i * 67) / 255.0;
      leds[i] = hot_color(heat * heat * heat, 1.0, 170);
    }

    for (int i = 0; i < N_LEDS; ++i) {
      float gradient = 1.0 - abs(float(i) / (N_LEDS - 1) - 0.5) * 2.0 + 1.0;
      float f = constrain(gradient * activation, 0.0, 1.0);
      f *= f;
      f *= (inoise8(clock_ms, module_index * 67, i * 67) / 255.0) * 0.3 + 0.7;
      CRGB col = hot_color(f * 0.7, 1.0);
      leds[i] += col;
    }
  }
};

Animation anim{0, leds, millis()};

void setupLeds() {
  FastLED.addLeds<WS2811, PIN_NEOPIXEL, BRG>(leds, N_LEDS)
    .setCorrection(CRGB(255, 210, 150));
}

void updateLeds() {
  // interactivity simulation
  int stage = millis() / 6000 % (N_MODULES + 1);
  unsigned long clock_ms = millis();

  Stats::start(Stats::KEY_ANIM);
  FastLED.clear();
  anim.update(clock_ms, stage);
  Stats::finish(Stats::KEY_ANIM);
  Stats::start(Stats::KEY_LED_UPDATE);
  FastLED.show();
  Stats::finish(Stats::KEY_LED_UPDATE);
}
