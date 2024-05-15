#ifndef ArduCAMWrapper_h
#define ArduCAMWrapper_h

#include <ArduCAM.h>

namespace SmartBin {
  namespace ArduCAMWrapper {
    struct Image {
      byte* data;
      size_t length;
    };

    ArduCAM init(int chipSelectPin);
    Image captureImage(ArduCAM* cam);
  }
}

#endif
