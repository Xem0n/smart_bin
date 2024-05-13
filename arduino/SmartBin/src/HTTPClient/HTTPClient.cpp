#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <SD.h>

#include "HTTPClient.h"

namespace SmartBin {
  HTTPClient::HTTPClient(const char* host, int port) {
    this->host = host;
    this->port = port;
  }

  HTTPClient::~HTTPClient() {
    // Destructor
  }

  void HTTPClient::sendRequest(File image) {
    if (client.connect(host, port)) {
      client.println("POST / HTTP/1.1");
      client.print("Host: ");
      client.println(host);
      client.println("Content-Type: image/jpeg");
      client.println("Content-Length: " + String(image.size()));
      client.println();
      writeImage(image);

      waitingForResponse = true;
    }
  }

  void HTTPClient::writeImage(File image) {
    while (true) {
      size_t size = image.available();

      if (size <= 0) {
        break;
      }

      size = size > sizeof(buffer) ? sizeof(buffer) : size;
      image.read(buffer, size);
      client.write(buffer, size);
    }
  }

  HTTPResponse HTTPClient::handleResponse() {
    if (!waitingForResponse) {
      return NO_REQUEST;
    }

    if (!client.connected()) {
      client.stop();
      waitingForResponse = false;

      return DISCONNECTED;
    }

    char* body = parseResponse();

    if (body != nullptr) {
      int type = strtol(body, nullptr, 10);

      delete[] body;

      return HTTPResponse(type);
    }

    return NO_BODY;
  }

  char* HTTPClient::parseResponse() {
    size_t bodyLength = 0;

    while (client.available()) {
      char* line = readLine();

      if (canReadBody(line, bodyLength)) {
        return getBody(bodyLength);
      }

      if (bodyLength == 0 && isContentLength(line)) {
        bodyLength = getContentLength(line);
      }

      delete[] line;

      waitingForResponse = false;
    }

    return nullptr;
  }

  char* HTTPClient::readLine() {
    char* line = new char[512];

    if (!line) {
      return nullptr;
    }

    size_t i = 0;
    char c = client.read();

    while (c != '\n' && i < 511) {
      line[i++] = c;
      c = client.read();
    }

    line[i] = '\0';

    return line;
  }

  bool HTTPClient::isContentLength(char* line) {
    return strstr(line, "Content-Length") != nullptr;
  }

  size_t HTTPClient::getContentLength(char* line) {
    char* length = strtok(line, " ");
    length = strtok(NULL, " ");

    return atoi(length);
  }

  bool HTTPClient::canReadBody(char* line, size_t bodyLength) {
    return strlen(line) == 1 && bodyLength > 0;
  }

  char* HTTPClient::getBody(size_t length) {
    char* body = new char[length + 1];

    if (!body) {
      return nullptr;
    }

    client.read((uint8_t*)body, length);

    body[length] = '\0';

    return body;
  }
}
