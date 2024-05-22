#include "Sensor.h"

#define MAX_DISTANCE 100 // in centimeters

namespace SmartBin {
  Sensor::Sensor(const int trigPin, const int echoPin) : trigPin(trigPin), echoPin(echoPin) {}

  Sensor::~Sensor() {
    // Destructor
  }

  void Sensor::init() {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
  }

  bool Sensor::detect() {
    long duration, distance;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(8);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = microsecondsToCentimeters(duration);

    return distance < MAX_DISTANCE;
  }

  long Sensor::microsecondsToCentimeters(long microseconds) {
    return (microseconds * 0.0343) / 2;
  }
}
