#include <Wire.h>

#define PERIPHERAL_ID 1
#define WIRE_TIMEOUT_US 3000

struct ControllerOwnedState {
  int on = 0;
};
struct PeripheralOwnedState {
  int8_t triggered = 0;
};

ControllerOwnedState cstate;
PeripheralOwnedState pstate;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin(PERIPHERAL_ID);
  Wire.setWireTimeout(WIRE_TIMEOUT_US, true);
  Wire.onReceive(&onReceive);
  Wire.onRequest(&onRequest);
  Serial.begin(9600);
  Serial.println("startup");
}

void onReceive(int nReceived) {
  if (nReceived != sizeof(ControllerOwnedState)) {
    return;
  }

  // receive controller-owned state
  uint8_t* cstateBytes = (uint8_t*)(&cstate);
  for (int i = 0; i < nReceived; ++i) {
    *cstateBytes = Wire.read();
    ++cstateBytes;
  }
  Serial.println();
}

void onRequest() {
  // send peripheral-owned state
  Wire.write((uint8_t*)&pstate, sizeof(PeripheralOwnedState));
}

void loop() {
  digitalWrite(LED_BUILTIN, cstate.on);
}
