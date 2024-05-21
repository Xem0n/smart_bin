#include <Arduino.h>
#include <SPI.h>
#include <WiFiNINA.h>
#include <SD.h>
#include <ArduCAM.h>
#include "../ArduCAMWrapper/ArduCAMWrapper.h"

#include "HTTPClient.h"

#define BUFFER_SIZE 64
#define TIMEOUT_LIMIT 5000

namespace SmartBin {
  HTTPClient::HTTPClient(const char* host, int port) {
    this->host = host;
    this->port = port;
  }

  HTTPClient::~HTTPClient() {
    // Destructor
  }

  void HTTPClient::sendRequest(ArduCAMWrapper::Image image) {
    Serial.println("Try sending...");
    if (client.connect(host, port)) {
      Serial.println("Connected!");
      client.println("POST / HTTP/1.1");
      client.print("Host: ");
      client.println(host);
      client.println("Content-Type: image/jpeg");
      client.println("Content-Length: " + String(image.length));
      client.println("Mac-Address: " + getMacAddress());
      client.println();
      writeImage(image);

      waitingForResponse = true;
    }
  }

  void HTTPClient::sendRequest(String filePath) {
    File file = SD.open(filePath, FILE_READ);

    Serial.println("Try sending...");
    if (client.connect(host, port)) {
      Serial.println("Connected!");
      client.println("POST / HTTP/1.1");
      client.print("Host: ");
      client.println(host);
      client.println("Content-Type: image/jpeg");
      client.println("Content-Length: " + String(file.size()));
      client.println("Mac-Address: " + getMacAddress());
      client.println();
      writeFile(file);

      waitingForResponse = true;
    }

    file.close();
  }

  void HTTPClient::writeImage(ArduCAMWrapper::Image image) {
    Serial.println("Writing image to server...");
    memset(buffer, 0, BUFFER_SIZE);
    size_t length = image.length;

    while (length > 0) {
      size_t size = min(BUFFER_SIZE, length);

      memcpy(buffer, image.data + image.length - length, size);
      client.write(buffer, size);

      length -= size;
    }
  }

  void HTTPClient::writeFile(File file) {
    Serial.println("Writing file to server...");
    memset(buffer, 0, BUFFER_SIZE);
    size_t length = file.size();

    while (length > 0) {
      size_t size = min(BUFFER_SIZE, length);

      file.read(buffer, size);
      client.write(buffer, size);

      length -= size;
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

    delete[] body;
    return WAITING;
  }

  char* HTTPClient::parseResponse() {
    size_t bodyLength = 0;

    while (client.available()) {
      char* line = readLine();

      if (canReadBody(line, bodyLength)) {
        delete[] line;
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

  String HTTPClient::getMacAddress() {
    byte mac[6];
    WiFi.macAddress(mac);

    String macAddress = "";

    for (int i = 5; i >= 0; i--) {
      macAddress += String(mac[i], HEX);

      if (i > 0) {
        macAddress += ":";
      }
    }

    return macAddress;
  }
}
