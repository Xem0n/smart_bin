#ifndef HttpClient_h
#define HttpClient_h

#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduCAM.h>
#include <SD.h>
#include "../ArduCAMWrapper/ArduCAMWrapper.h"

namespace SmartBin {
  enum ResponseType {
    OK,
    UNKNOWN_TYPE,
    NO_REQUEST,
    DISCONNECTED,
    WAITING,
    TIMEOUT,
    NO_BODY,
  };

  struct HTTPResponse {
    ResponseType type;
    char* body;
  };

  class HTTPClient {
  public:
    HTTPClient(const char* host, int port);
    ~HTTPClient();

    void getColor();
    void sendImage();
    void sendImage(ArduCAMWrapper::Image image);
    void sendImage(String filePath);
    HTTPResponse handleResponse();

  private:
    const char* host;
    int port;
    WiFiClient client;
    byte buffer[64];
    bool waitingForResponse = false;
    size_t sentTimestamp = 0;

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
