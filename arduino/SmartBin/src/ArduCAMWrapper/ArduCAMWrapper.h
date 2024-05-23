#ifndef ArduCAMWrapper_h
#define ArduCAMWrapper_h

#include <ArduCAM.h>

#define MAX_IMAGE_SIZE 20480

namespace SmartBin {
  namespace ArduCAMWrapper {
    struct Image {
      // byte* data = nullptr;
      byte data[MAX_IMAGE_SIZE];
      size_t length = 0;
    };

    ArduCAM init(int chipSelectPin);
    Image captureImage(ArduCAM* cam);
    String saveImage(ArduCAM* cam);
  }
}

#endif
