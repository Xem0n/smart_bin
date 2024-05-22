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

// todo:
//  - repeat request for bin color

char ssid[] = "airbag";
char pass[] = "glebogryzarka";

HTTPClient httpClient("192.168.109.106", 5000);
ArduCAM myCam;
StepperController stepperController;

size_t lastUpdateTime = 0;
String lastImagePath = "";
uint8_t binColor[3] = {255, 255, 255};

const char *garbageTypeMapping[] = {
  "metal",
  "paper",
  "plastic"
};

enum LoopState {
  LOOP_IDLE,
  LOOP_SEND_IMAGE,
  LOOP_WAIT_FOR_GARBAGE_TYPE,
  LOOP_WAIT_FOR_COLOR,
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
    case LOOP_WAIT_FOR_GARBAGE_TYPE:
      handleResponse(LOOP_SEND_IMAGE, LOOP_IDLE, receiveGarbageType);
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

  loopState = LOOP_WAIT_FOR_GARBAGE_TYPE;
}

void sendImageFromSD() {
  Serial.println("Sending image from SD...");

  if (lastImagePath != "") {
    httpClient.sendImage(lastImagePath);

    loopState = LOOP_WAIT_FOR_GARBAGE_TYPE;
    return;
  }

  String path = ArduCAMWrapper::saveImage(&myCam);

  if (path == "") {
    return;
  }

  lastImagePath = path;
  loopState = LOOP_WAIT_FOR_GARBAGE_TYPE;

  httpClient.sendImage(path);
}

void handleResponse(LoopState fallbackState, LoopState successState, void (*callback)(HTTPResponse)) {
  HTTPResponse response = httpClient.handleResponse();

  switch (response.type) {
    case DISCONNECTED:
      Serial.println("Disconnected. Sending once more.");

      loopState = fallbackState;
      lastUpdateTime = millis();

      break;
    case TIMEOUT:
      Serial.println("Timeout. Sending once more.");

      loopState = fallbackState;
      lastUpdateTime = millis();

      break;
    case NO_REQUEST:
      Serial.println("to sie tez nie powinno zdarzyc xd. nie wiem jak to obsluzyc");

      lastImagePath = "";
      lastUpdateTime = millis();
      loopState = successState;

      break;
    case NO_BODY:
      Serial.println("No body. Probably 400.");
      break;
    case WAITING:
      break;
    case UNKNOWN_TYPE:
      Serial.println("Cos poszlo nie tak xd");
      break;
    case OK:
      Serial.print("Response: ");
      Serial.println(response.body);

      (*callback)(response);

      lastImagePath = "";
      lastUpdateTime = millis();
      loopState = successState;

      break;
    default:
      break;
  }

  delete[] response.body;
}

void receiveGarbageType(HTTPResponse response) {
  int type = strtol(response.body, nullptr, 10);

  if (isGarbageType(type)) {
    handleGarbage(type);
  }
}

void receiveBinColor(HTTPResponse response) {
  // ...
}

bool isGarbageType(int garbageType) {
  return garbageType >= 1 && garbageType <= 3;
}

void handleGarbage(int garbageType) {
    Serial.println(garbageTypeMapping[garbageType - 1]);

    // stepperController.drop(garbageType - 1);
}
