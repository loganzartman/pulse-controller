#pragma once

#ifdef AVR
  #pragma message "Using Arduino Nano pinout"
  #define PIN_LED_BUILTIN 13
  #define PIN_PIR 4
  #define PIN_STEPPER1_STEP 5
  #define PIN_STEPPER1_DIR 6
  #define PIN_STEPPER2_STEP 7
  #define PIN_STEPPER2_DIR 8
  #define PIN_NEOPIXEL 9
#else
#ifdef ESP32
  #pragma message "Using ESP32 Devkit V1 pinout"
  #define PIN_LED_BUILTIN 2
  #define PIN_PIR 13
  #define PIN_STEPPER1_STEP 12
  #define PIN_STEPPER1_DIR 14
  #define PIN_STEPPER2_STEP 27
  #define PIN_STEPPER2_DIR 26
  #define PIN_NEOPIXEL 25
#else
  #error Unknown platform
#endif
#endif
