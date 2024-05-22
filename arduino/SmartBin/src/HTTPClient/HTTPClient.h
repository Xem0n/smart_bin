#ifndef HttpClient_h
#define HttpClient_h

#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduCAM.h>
#include <SD.h>
#include "../ArduCAMWrapper/ArduCAMWrapper.h"

namespace SmartBin {
  enum HTTPResponse {
    UNKNOWN_TYPE = 0,
    METAL = 1,
    PAPER = 2,
    PLASTIC = 3,
    NO_REQUEST,
    DISCONNECTED,
    WAITING,
    TIMEOUT,
    NO_BODY,
  };

  class HTTPClient {
  public:
    HTTPClient(const char* host, int port);
    ~HTTPClient();

    void sendImage(ArduCAMWrapper::Image image);
    void sendImage(String filePath);
    HTTPResponse handleResponse();

  private:
    const char* host;
    int port;
    WiFiClient client;
    byte buffer[64];
    bool waitingForResponse = false;
    size_t sentTimestamp;

    void writeImage(ArduCAMWrapper::Image image);
    void writeFile(File file);
    char* readLine();
    char* parseResponse();
    char* getBody(size_t length);
    bool canReadBody(char* line, size_t bodyLength);
    bool isContentLength(char* line);
    size_t getContentLength(char* line);
    String getMacAddress();
  };
}

#endif
