#include <Wire.h>

#define PERIPHERAL_ADDRESS_UNUSED 123
#define WIRE_TIMEOUT_US 3000

int currentAddress = PERIPHERAL_ADDRESS_UNUSED;

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

  Wire.begin(currentAddress);
  Wire.setWireTimeout(WIRE_TIMEOUT_US, true);
  Wire.onReceive(&onReceive);
  Wire.onRequest(&onRequest);

  Serial.begin(9600);
  Serial.println("pulse peripheral");
  Serial.println("Send command 'a X' to set I2C port to X.");
}

void setAddress(int newAddress) {
  Wire.end();
  currentAddress = newAddress;
  Wire.begin(currentAddress);
  Serial.print("Reconnected with address: ");
  Serial.println(currentAddress);
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
}

void onRequest() {
  // send peripheral-owned state
  Wire.write((uint8_t*)&pstate, sizeof(PeripheralOwnedState));
}

void updateSerial() {
  if (!Serial.available()) {
    return;
  }
  String command = Serial.readStringUntil(' ');
  command.trim();
  if (command.equals("a")) {
    String newAddressStr = Serial.readString();
    int newAddress = newAddressStr.toInt();
    if (newAddressStr.length() == 0) {
      Serial.print("Current address: ");
      Serial.println(currentAddress);
    } else if (newAddress == 0) {
      Serial.print("Invalid address: ");
      Serial.println(newAddressStr);
    } else {
      setAddress(newAddress);
    }
  } else {
    Serial.print("Invalid command: '");
    Serial.print(command);
    Serial.println("'");
  }
}

void loop() {
  digitalWrite(LED_BUILTIN, cstate.on);
  updateSerial();
}