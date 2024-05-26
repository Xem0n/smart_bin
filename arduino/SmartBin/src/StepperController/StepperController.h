#ifndef StepperController_h
#define StepperController_h

#include <Arduino.h>
#include <Stepper.h>

namespace SmartBin {

  class StepperController {
  public:
    StepperController();
    ~StepperController();

    void drop(int garbageType);

    void dropMetal();
    void dropPaper();
    void dropPlastic();

  private:
    Stepper* upperStepper = new Stepper(1024, A6, A4, A5, A3);
    Stepper* lowerStepper = new Stepper(1024, 3, 1, 2, 0);
  };
}

#endif
