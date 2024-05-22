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

size_t lastUpdateTime = 0;
String lastImagePath = "";

const char *garbageTypeMapping[] = {
  "metal",
  "paper",
  "plastic"
};

enum LoopState {
  LOOP_IDLE,
  LOOP_SEND_IMAGE,
  LOOP_HANDLE_RESPONSE,
};

LoopState loopState = LOOP_IDLE;

void dropTest() {
  stepperController.drop(0);
  delay(1000);
  stepperController.drop(1);
  delay(1000);
  stepperController.drop(2);
}

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

void loop() {
  switch (loopState) {
    case LOOP_IDLE:
      idle();
      break;
    case LOOP_SEND_IMAGE:
      sendImage();
      // sendImageFromSD();
      break;
    case LOOP_HANDLE_RESPONSE:
      handleResponse();
      break;
    default:
      break;
  }
}

void idle() {
  size_t timestamp = millis();

  if ((timestamp - lastUpdateTime) > SEND_INTERVAL) {
    Serial.println("10s idle, sending image");

    loopState = LOOP_SEND_IMAGE;
    lastUpdateTime = timestamp;
  }
}

void sendImage() {
  Serial.println("Sending image...");

  ArduCAMWrapper::Image image = ArduCAMWrapper::captureImage(&myCam);
  httpClient.sendImage(image);

  delete[] image.data;

  loopState = LOOP_HANDLE_RESPONSE;
}

void sendImageFromSD() {
  Serial.println("Sending image from SD...");

  if (lastImagePath != "") {
    httpClient.sendImage(lastImagePath);

    loopState = LOOP_HANDLE_RESPONSE;
    return;
  }

  String path = ArduCAMWrapper::saveImage(&myCam);

  if (path == "") {
    return;
  }

  lastImagePath = path;
  loopState = LOOP_HANDLE_RESPONSE;

  httpClient.sendImage(path);
}

void handleResponse() {
  HTTPResponse response = httpClient.handleResponse();

  switch (response) {
    case DISCONNECTED:
      Serial.println("Disconnected. Sending once more.");

      loopState = LOOP_SEND_IMAGE;
      lastUpdateTime = millis();

      break;
    case TIMEOUT:
      Serial.println("Timeout. Sending once more.");

      loopState = LOOP_SEND_IMAGE;
      lastUpdateTime = millis();

      break;
    case NO_REQUEST:
      Serial.println("to sie tez nie powinno zdarzyc xd");

      lastImagePath = "";
      lastUpdateTime = millis();
      loopState = LOOP_IDLE;

      break;
    case NO_BODY:
      Serial.println("No body. Probably 400.");
      break;
    case WAITING:
      break;
    case UNKNOWN_TYPE:
      Serial.println("Cos poszlo nie tak xd");
      break;
    default:
      Serial.print("Response: ");
      Serial.println(response);

      if (isGarbageType(response)) {
        handleGarbage(response);
      }

      lastImagePath = "";
      lastUpdateTime = millis();
      loopState = LOOP_IDLE;

      break;
  }
}

bool isGarbageType(HTTPResponse response) {
  return response >= 1 && response <= 3;
}

void handleGarbage(HTTPResponse response) {
    Serial.println(garbageTypeMapping[response - 1]);

    // stepperController.drop((int)response - 1);
}
