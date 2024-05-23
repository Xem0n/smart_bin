#ifndef ArduCAMWrapper_h
#define ArduCAMWrapper_h

#include <ArduCAM.h>

namespace SmartBin {
  namespace ArduCAMWrapper {
    struct Image {
      byte* data = nullptr;
      size_t length = 0;
    };

    ArduCAM init(int chipSelectPin);
    Image captureImage(ArduCAM* cam);
    String saveImage(ArduCAM* cam);
  }
}

#endif
