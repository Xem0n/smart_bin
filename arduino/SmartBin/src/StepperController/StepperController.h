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
    Stepper* upperStepper = new Stepper(1024, 5, 3, 4, 2);
    Stepper* lowerStepper = new Stepper(1024, 1, A6, 0, A5);
  };
}

#endif
