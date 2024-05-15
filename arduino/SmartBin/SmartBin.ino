#include <Wire.h>
#include <SPI.h>
#include <ArduCAM.h>
#include <WiFiNINA.h>
#include <SD.h>

#include "src/WiFiWrapper/WiFiWrapper.h"
#include "src/SDWrapper/SDWrapper.h"
#include "src/ArduCAMWrapper/ArduCAMWrapper.h"
#include "src/HTTPClient/HTTPClient.h"

using namespace SmartBin;

int SD_PIN = A1;
int ARDUCAM_PIN = 0;

char ssid[] = "ZielonePomarancze";
char pass[] = "MurzynskieBananki";

HTTPClient httpClient("192.168.100.208", 5000);
ArduCAM myCam;

void setup() {
  Serial.begin(9600);

  while (!Serial) {
    ;
  }

  Serial.println("Initializing...");

  Wire.begin();
  SPI.begin();
  delay(3000);

  myCam = ArduCAMWrapper::init(ARDUCAM_PIN);

  if (!SDWrapper::init(SD_PIN) || !WiFiWrapper::init(ssid, pass)) {
    Serial.println("Initialization failed!");
    delay(60000);
  }

  Serial.println("Initialization done!");
  Serial.println();

  sendImage();
}

void sendImage() {
  // File image = SD.open("metal.jpg", FILE_READ);
  // File image = SD.open("paper.jpg", FILE_READ);
  // File image = SD.open("plastic.jpg", FILE_READ);
  // File image = SD.open("large_~1.png", FILE_READ);

  Serial.println("Sending image...");

  ArduCAMWrapper::Image image = ArduCAMWrapper::captureImage(&myCam);

  httpClient.sendRequest(image);

  delete[] image.data;

  // image.close();
}

bool isGarbageType(HTTPResponse response) {
  return response >= 1 && response <= 3;
}

void loop() {
  HTTPResponse response = httpClient.handleResponse();

  if (response == DISCONNECTED) {
    Serial.println("Try once more...");
    sendImage();
  } else if (isGarbageType(response)) {
    Serial.println("Response:");
    Serial.println(response);
  }
}
