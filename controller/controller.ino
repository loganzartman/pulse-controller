#include <Wire.h>
#include "LED.h"

#define PERIPHERAL_COUNT 2
#define WIRE_TIMEOUT_US 3000
#define WIRE_CLOCK 100000
#define PERIPHERAL_ADDRESS(index) (index+1)

struct ControllerOwnedState {
  uint32_t clockMs = 0;
  uint8_t on = 0;
};
struct PeripheralOwnedState {
  int8_t triggered = 0;
};

ControllerOwnedState cstates[PERIPHERAL_COUNT];
PeripheralOwnedState pstates[PERIPHERAL_COUNT];

void setup() {
  pinMode(PIN_LED_BUILTIN, OUTPUT);
  Wire.setClock(WIRE_CLOCK);
  Wire.begin();
#ifdef WIRE_HAS_TIMEOUT
  Wire.setWireTimeout(WIRE_TIMEOUT_US, true);
#elif defined(ESP32)
  // at time of writing, esp32 wire.h only has old setTimeout interface
  Wire.setTimeout(WIRE_TIMEOUT_US / 1000);
#endif
  Serial.begin(9600);
  Serial.println("startup");
#if defined(ESP32)
  Serial.setDebugOutput(true);
#endif

  pinMode(PIN_NEOPIXEL, OUTPUT);

  setupLeds();
}

void syncState() {
  digitalWrite(PIN_LED_BUILTIN, cstates[0].on);
  for (int i = 0; i < PERIPHERAL_COUNT; ++i) {
    // send controller-owned state
    Wire.beginTransmission(PERIPHERAL_ADDRESS(i));
    Wire.write((uint8_t*)(&cstates[i]), sizeof(ControllerOwnedState));
    Wire.endTransmission();
    
    // receive peripheral-owned state
    int nReceived = Wire.requestFrom(PERIPHERAL_ADDRESS(i), sizeof(PeripheralOwnedState));
    if (nReceived != sizeof(PeripheralOwnedState)) {
      continue;
    }
    uint8_t* pstateBytes = (uint8_t*)(&pstates[i]);
    for (int j = 0; j < sizeof(PeripheralOwnedState); ++j) {
      *pstateBytes = Wire.read();
      ++pstateBytes;
    }
  }
}

void loop() {
  uint32_t clockMs = millis();
  for (int i = 0; i < PERIPHERAL_COUNT; ++i) {
    cstates[i].clockMs = clockMs;
    cstates[i].on = pstates[i].triggered;
  }

  syncState();

  updateLeds();
}
