#include <AccelStepper.h>
#include "pinout.h"

// max without skipping @ no load ~600
#define STEPPER_MAX_SPEED 600.0
#define STEPPER_ACCELERATION 300.0

#define STEPPER_TRAVEL_STEPS 2000

AccelStepper lstepper(AccelStepper::DRIVER, PIN_STEPPER1_STEP, PIN_STEPPER1_DIR);
AccelStepper rstepper(AccelStepper::DRIVER, PIN_STEPPER2_STEP, PIN_STEPPER2_DIR);

void setupSteppers() {
  lstepper.setMaxSpeed(STEPPER_MAX_SPEED);
  rstepper.setMaxSpeed(STEPPER_MAX_SPEED);
  lstepper.setAcceleration(STEPPER_ACCELERATION);
  rstepper.setAcceleration(STEPPER_ACCELERATION);
}

void lowerSteppers() {
  lstepper.moveTo(0);
  rstepper.moveTo(0);
}

void raiseSteppers() {
  lstepper.moveTo(STEPPER_TRAVEL_STEPS);
  rstepper.moveTo(STEPPER_TRAVEL_STEPS);
}

void updateSteppers() {
  lstepper.run();
  rstepper.run();
}
