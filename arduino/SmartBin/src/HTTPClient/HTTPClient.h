#ifndef HttpClient_h
#define HttpClient_h

#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <SD.h>

namespace SmartBin {
  enum HTTPResponse {
    UNKNOWN_TYPE = 0,
    METAL = 1,
    PAPER = 2,
    PLASTIC = 3,
    NO_REQUEST,
    DISCONNECTED,
    NO_BODY
  };

  class HTTPClient {
  public:
    HTTPClient(const char* host, int port);
    ~HTTPClient();

    void sendRequest(File image);
    HTTPResponse handleResponse();

  private:
    const char* host;
    int port;
    WiFiClient client;
    byte buffer[512];
    bool waitingForResponse = false;

    void writeImage(File image);
    char* readLine();
    char* parseResponse();
    char* getBody(size_t length);
    bool canReadBody(char* line, size_t bodyLength);
    bool isContentLength(char* line);
    size_t getContentLength(char* line);
  };
}

#endif
