#ifndef Sensor_h
#define Sensor_h

#include <Arduino.h>

namespace SmartBin {
  class Sensor {
  public:
    Sensor(const int trigPin, const int echoPin);
    ~Sensor();

    void init();
    bool detect();

  private:
    const int trigPin;
    const int echoPin;

    long microsecondsToCentimeters(long microseconds);
  };
}

#endif
