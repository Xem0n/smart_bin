#include <Wire.h>
#include <SPI.h>
#include <ArduCAM.h>

#include "src/WiFiWrapper/WiFiWrapper.h"
#include "src/SDWrapper/SDWrapper.h"
#include "src/ArduCAMWrapper/ArduCAMWrapper.h"
#include "src/HTTPClient/HTTPClient.h"
#include "src/StepperController/StepperController.h"

using namespace SmartBin;

#define SD_PIN A1
#define ARDUCAM_PIN A4

#define SEND_INTERVAL 10000

char ssid[] = "airbag";
char pass[] = "glebogryzarka";

HTTPClient httpClient("192.168.109.106", 5000);
ArduCAM myCam;
StepperController stepperController;

size_t lastSendTime = millis();

const char *garbageTypeMapping[] = {
  "metal",
  "paper",
  "plastic"
};

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
}

void dropTest() {
  stepperController.drop(0);
  delay(1000);
  stepperController.drop(1);
  delay(1000);
  stepperController.drop(2);
}

void sendImage() {
  Serial.println("Sending image...");

  ArduCAMWrapper::Image image = ArduCAMWrapper::captureImage(&myCam);
  httpClient.sendRequest(image);

  delete[] image.data;
}

bool isGarbageType(HTTPResponse response) {
  return response >= 1 && response <= 3;
}

void loop() {
  HTTPResponse response = httpClient.handleResponse();

  if (response == DISCONNECTED) {
    Serial.println("Try once more...");
    sendImage();

    lastSendTime = millis();
  } else if (isGarbageType(response)) {
    Serial.print("Response: ");
    Serial.println(garbageTypeMapping[response - 1]);

    // stepperController.drop((int)response - 1);
    lastSendTime = millis();
  } else if (response == NO_REQUEST) {
    size_t timestamp = millis();

    if ((timestamp - lastSendTime) > SEND_INTERVAL) {
      Serial.println("10s idle, sending image");
      sendImage();
      lastSendTime = timestamp;
    }
  } else if (response == UNKNOWN_TYPE) {
    Serial.println("Unknown type");
  }
}
