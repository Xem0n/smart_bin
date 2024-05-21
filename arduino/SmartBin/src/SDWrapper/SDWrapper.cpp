#include <SD.h>

#include "SDWrapper.h"

bool SmartBin::SDWrapper::init(int chipSelectPin) {
  if (!SD.begin(chipSelectPin)) {
    Serial.println("SD card initialization failed.");

    return false;
  }

  return true;
}
