#include <Wire.h>
#include "pinout.h"
#include "PIR.h"
#include "Stepper.h"

#define PERIPHERAL_ADDRESS 1
#define WIRE_TIMEOUT_US 3000
#define WIRE_CLOCK 100000

struct ControllerOwnedState {
  uint32_t clockMs = 0;
  uint8_t on = 0;
};
struct PeripheralOwnedState {
  int8_t triggered = 0;
};

ControllerOwnedState cstate;
PeripheralOwnedState pstate;
volatile ControllerOwnedState cstateIncoming;

PIR pir{PIN_PIR};

void setup() {
  pinMode(PIN_LED_BUILTIN, OUTPUT);

  Wire.begin(PERIPHERAL_ADDRESS);
  Wire.setClock(WIRE_CLOCK);
#ifdef WIRE_HAS_TIMEOUT
  Wire.setWireTimeout(WIRE_TIMEOUT_US, true);
#endif
  Wire.onReceive(&onReceive);
  Wire.onRequest(&onRequest);

  Serial.begin(9600);
  Serial.println("pulse peripheral");
  Serial.println("Send command 'a X' to set I2C port to X.");

  pir.onMotion(&onMotion);
  pir.onMotionStop(&onMotionStop);

  setupSteppers();
}

void onReceive(int nReceived) {
  if (nReceived != sizeof(ControllerOwnedState)) {
    return;
  }

  // receive controller-owned state
  uint8_t* cstateBytes = (uint8_t*)(&cstateIncoming);
  for (int i = 0; i < nReceived; ++i) {
    *cstateBytes = Wire.read();
    ++cstateBytes;
  }
}

void onRequest() {
  // send peripheral-owned state
  Wire.write((uint8_t*)&pstate, sizeof(PeripheralOwnedState));
}

void onMotion() {
  Serial.println("MOTION");
  pstate.triggered = 1;
  raiseSteppers();
}

void onMotionStop() {
  Serial.println("STOP");
  pstate.triggered = 0;
  lowerSteppers();
}

void updateSerial() {
  if (!Serial.available()) {
    return;
  }
  String command = Serial.readStringUntil(' ');
  command.trim();
  if (command.equals("i")) {
    Serial.println("up");
  } else {
    Serial.print("Invalid command: '");
    Serial.print(command);
    Serial.println("'");
  }
}

void syncState() {
  noInterrupts();
  cstate = *const_cast<ControllerOwnedState*>(&cstateIncoming);
  interrupts();
}

void loop() {
  syncState();
  updateSerial();
  pir.update();
  digitalWrite(PIN_LED_BUILTIN, pstate.triggered);
  updateSteppers();
}
