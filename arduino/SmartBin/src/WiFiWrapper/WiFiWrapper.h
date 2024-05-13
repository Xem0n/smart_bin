#ifndef WiFiWrapper_h
#define WiFiWrapper_h

namespace SmartBin {
  namespace WiFiWrapper {
    bool init(char* ssid, char* pass);
    void printWifiStatus();
  }
}

#endif
