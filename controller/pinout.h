#if defined(AVR)
  #pragma message "Using Arduino Nano pinout"
  #define PIN_LED_BUILTIN 13
  #define PIN_SDA A4
  #define PIN_SCL A5
  #define PIN_NEOPIXEL 6
#elif defined(ESP32)
  #pragma message "Using ESP32 Devkit V1 pinout"
  #define PIN_LED_BUILTIN 2
  #define PIN_SDA 22
  #define PIN_SCL 21
  #define PIN_NEOPIXEL 27
#else
  #error Unknown platform
#endif
