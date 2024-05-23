#include <Wire.h>
#include <SPI.h>
#include <ArduCAM.h>
#include <RGBLed.h>

#include "src/WiFiWrapper/WiFiWrapper.h"
#include "src/SDWrapper/SDWrapper.h"
#include "src/ArduCAMWrapper/ArduCAMWrapper.h"
#include "src/HTTPClient/HTTPClient.h"
#include "src/StepperController/StepperController.h"
#include "src/Sensor/Sensor.h"

using namespace SmartBin;

#define SD_PIN A0
#define ARDUCAM_PIN A4

#define REQUEST_INTERVAL 60000 // in miliseconds
#define SEND_INTERVAL 10000 // in miliseconds

char ssid[] = "airbag";
char pass[] = "glebogryzarka";

HTTPClient httpClient("192.168.109.106", 5000);
RGBLed mainLed(7, 8, 9, false);
ArduCAM myCam;
StepperController stepperController;
Sensor sensor(7, 6);

size_t lastUpdateTime = 0;
size_t lastRequestTime = 0;

ArduCAMWrapper::Image lastImage;
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
  LOOP_REQUEST_COLOR,
  LOOP_WAIT_FOR_COLOR,
};

LoopState loopState = LOOP_REQUEST_COLOR;

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

  mainLed.setColor(binColor[0], binColor[1], binColor[2]);
  myCam = ArduCAMWrapper::init(ARDUCAM_PIN);
  sensor.init();

  // if (!SDWrapper::init(SD_PIN) || !WiFiWrapper::init(ssid, pass)) {
  if (!WiFiWrapper::init(ssid, pass)) {
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
    case LOOP_REQUEST_COLOR:
      requestColor();
      break;
    case LOOP_WAIT_FOR_GARBAGE_TYPE:
      handleResponse(LOOP_SEND_IMAGE, LOOP_IDLE, receiveGarbageType);
      break;
    case LOOP_WAIT_FOR_COLOR:
      handleResponse(LOOP_REQUEST_COLOR, LOOP_IDLE, receiveBinColor);
      break;
    default:
      break;
  }
}

void idle() {
  size_t timestamp = millis();

  // if ((timestamp - lastUpdateTime) > SEND_INTERVAL) {
  //   Serial.println("10s idle, sending image");
  //
  //   loopState = LOOP_SEND_IMAGE;
  //   lastUpdateTime = timestamp;
  // }

  if (sensor.detect()) {
    Serial.println("Detected.");

    // ensure that there's not gonna be a memory leak
    // resetImage();

    loopState = LOOP_SEND_IMAGE;
  } else if ((timestamp - lastRequestTime) > REQUEST_INTERVAL) {
    Serial.println("60s elapsed. Requesting color...");

    loopState = LOOP_REQUEST_COLOR;
    lastRequestTime = timestamp;
  } else {
    delay(5);
  }
}

void sendImage() {
  Serial.println("Sending image...");

  if (lastImage.length == 0) {
    Serial.println("No image data.");
    lastImage = ArduCAMWrapper::captureImage(&myCam);
  }

  httpClient.sendImage(lastImage);

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

void requestColor() {
  httpClient.getColor();

  loopState = LOOP_WAIT_FOR_COLOR;
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

      Serial.println("Reset loop state.");

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

  resetImage();
}

void resetImage() {
  Serial.println("Restting image data.");

  lastImage.length = 0;

  if (lastImage.data != nullptr) {
    delete[] lastImage.data;
    lastImage.data = nullptr;
  }
}

void receiveBinColor(HTTPResponse response) {
  char colorHex[3];

  for (int i = 0; i < 3; i++) {
    memcpy(colorHex, &response.body[i * 2 + 1], 2);
    colorHex[2] = '\0';

    binColor[i] = strtol(colorHex, nullptr, 16);
  }

  Serial.print("Bin color: ");
  Serial.print(binColor[0]);
  Serial.print(", ");
  Serial.print(binColor[1]);
  Serial.print(", ");
  Serial.println(binColor[2]);

  mainLed.setColor(binColor[0], binColor[1], binColor[2]);
}

bool isGarbageType(int garbageType) {
  return garbageType >= 1 && garbageType <= 3;
}

void handleGarbage(int garbageType) {
  Serial.println(garbageTypeMapping[garbageType - 1]);

  stepperController.drop(garbageType - 1);
  Serial.println("spadnelo");
}
