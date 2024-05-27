#include <Stepper.h>

#include "StepperController.h"

namespace SmartBin {
  typedef void (StepperController::*method_function)();

  method_function garbageTypeToMethodMapping[3] = {
    &StepperController::dropMetal,
    &StepperController::dropPaper,
    &StepperController::dropPlastic
  };

  StepperController::StepperController() {
    upperStepper->setSpeed(12);
    lowerStepper->setSpeed(12);
  }

  StepperController::~StepperController() {
    // Destructor
  }

  void StepperController::drop(int garbageType) {
    (this->*garbageTypeToMethodMapping[garbageType])();
  }

  void StepperController::dropMetal() {
    Serial.println("Open metal...");

    upperStepper->step(-424);
    delay(1000);
    upperStepper->step(424);
  }

  void StepperController::dropPaper() {
    Serial.println("Open paper...");

    upperStepper->step(480);
    delay(1000);
    upperStepper->step(-480);
  }

  void StepperController::dropPlastic() {
    Serial.println("Open plastic...");

    lowerStepper->step(-608);
    delay(10);
    upperStepper->step(352);
    delay(1000);
    upperStepper->step(-352);
    delay(10);
    lowerStepper->step(608);
  }
}
