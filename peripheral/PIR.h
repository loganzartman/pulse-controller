#define PIN_PIR 4

struct PIR {
  int pin;
  int motion = 0;
  void (*motionCallback)(void);
  void (*motionStopCallback)(void);

  PIR(int pin) : pin(pin) {
    pinMode(pin, INPUT);
  }

  void onMotion(void (*fn)(void)) {
    motionCallback = fn;
  }

  void onMotionStop(void (*fn)(void)) {
    motionStopCallback = fn;
  }

  void update() {
    int pirState = digitalRead(pin);
    if (!motion && pirState == HIGH) {
      motion = 1;
      motionCallback();
    }
    if (motion && pirState == LOW) {
      motion = 0;
      motionStopCallback();
    }
  }
};
