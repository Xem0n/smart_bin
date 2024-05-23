#include <SPI.h>
#include <ArduCAM.h>
#include <SD.h>

#include "ArduCAMWrapper.h"

#define BUFFER_SIZE 256

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__

int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

ArduCAM SmartBin::ArduCAMWrapper::init(int chipSelectPin) {
  ArduCAM myCAM(OV2640, chipSelectPin);

  Serial.println("ArduCAM start initialization...");

  pinMode(chipSelectPin, OUTPUT);
  digitalWrite(chipSelectPin, HIGH);

  myCAM.write_reg(0x07, 0x80);
  delay(100);
  myCAM.write_reg(0x07, 0x00);
  delay(100);

  myCAM.write_reg(ARDUCHIP_TEST1, 0x55);
  uint8_t temp = myCAM.read_reg(ARDUCHIP_TEST1);

  if (temp != 0x55) {
    Serial.println(F("SPI interface Error!"));
  } else {
    Serial.println(F("SPI interface OK."));
  }

  uint8_t vid, pid;
  myCAM.wrSensorReg8_8(0xff, 0x01);
  myCAM.rdSensorReg8_8(OV2640_CHIPID_HIGH, &vid);
  myCAM.rdSensorReg8_8(OV2640_CHIPID_LOW, &pid);

  if ((vid != 0x26 ) && (( pid != 0x41 ) || ( pid != 0x42 ))) {
    Serial.println(F("Can't find OV2640 module!"));
    delay(1000);
  } else {
    Serial.println(F("OV2640 detected."));
  }

  myCAM.set_format(JPEG);
  myCAM.InitCAM();
  myCAM.OV2640_set_JPEG_size(OV2640_640x480);
  // myCAM.OV2640_set_Light_Mode(Auto);
  myCAM.OV2640_set_Color_Saturation(Saturation2);
  myCAM.OV2640_set_Brightness(Brightness_2);
  myCAM.OV2640_set_Contrast(Contrast4);
  // myCAM.OV2640_set_Special_effects();
  delay(1000);
  myCAM.clear_fifo_flag();

  return myCAM;
}

SmartBin::ArduCAMWrapper::Image SmartBin::ArduCAMWrapper::captureImage(ArduCAM* cam) {
  SmartBin::ArduCAMWrapper::Image image;

  Serial.println("try capturing...");

  cam->flush_fifo();
  cam->clear_fifo_flag();
  cam->start_capture();

  Serial.println("waiting...");
  while (!cam->get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
  Serial.println("begin...");

  byte buffer[BUFFER_SIZE];
  size_t length = cam->read_fifo_length();
  size_t initLength = length;
  size_t offset = 0;
  size_t i = 0;
  uint8_t temp = 0, temp_last = 0;
  bool is_header = false;

  Serial.print("Free ram: ");
  Serial.print((float)freeMemory() / 1024);
  Serial.println(" kb");

  Serial.println("Allocate image data...");

  image.data = new byte[length];
  // image.data = (byte*)realloc(image.data == nullptr ? NULL : image.data, length);

  if (image.data == nullptr) {
    Serial.println("Failed to allocate memory for image data");
    return image;
  }

  image.length = length;

  Serial.print("Image size: ");
  Serial.print(length / 1024);
  Serial.println(" kb");

  Serial.print("Free ram: ");
  Serial.print((float)freeMemory() / 1024);
  Serial.println(" kb");

  cam->CS_LOW();
  cam->set_fifo_burst();

  while (length--) {
    temp_last = temp;
    temp = SPI.transfer(0x00);
    //Read JPEG data from FIFO
    if ((temp == 0xD9) && (temp_last == 0xFF)) { //If find the end, break while
      buffer[i++] = temp;  //save the last  0XD9
      //Write the remain bytes in the buffer
      cam->CS_HIGH();
      memcpy(image.data + offset, buffer, i);
      image.length = initLength - length;
      is_header = false;
      i = 0;
    }

    if (is_header == true) {
       //Write image data to buffer if not full
      if (i < BUFFER_SIZE) {
        buffer[i++] = temp;
      } else {
        //Write 256 bytes image data to client
        cam->CS_HIGH();
        memcpy(image.data + offset, buffer, BUFFER_SIZE);
        offset += BUFFER_SIZE;
        i = 0;
        buffer[i++] = temp;
        cam->CS_LOW();
        cam->set_fifo_burst();
      }
    } else if ((temp == 0xD8) & (temp_last == 0xFF)) {
      is_header = true;
      buffer[i++] = temp_last;
      buffer[i++] = temp;
    }
  }

  return image;
}

String SmartBin::ArduCAMWrapper::saveImage(ArduCAM* cam) {
  static int index = 0;

  String filename = "/" + String(index++) + ".jpg";
  File file = SD.open(filename, FILE_WRITE);

  if (!file) {
    Serial.println("Failed to open file for writing");
    return "";
  }

  Serial.println("try capturing...");

  cam->flush_fifo();
  cam->clear_fifo_flag();
  cam->start_capture();

  Serial.println("waiting...");
  while(!cam->get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));
  Serial.println("begin...");

  byte buffer[BUFFER_SIZE];
  size_t length = cam->read_fifo_length();
  size_t i = 0;
  uint8_t temp = 0, temp_last = 0;
  bool is_header = false;

  Serial.print("Image size: ");
  Serial.print(length / 1024);
  Serial.println(" kb");

  if (length >= MAX_FIFO_SIZE) { // 384 kb
    Serial.println(F("Over size."));
    return "";
  } else if (length == 0) {
    Serial.println(F("Size is 0."));
    return "";
  }

  cam->CS_LOW();
  cam->set_fifo_burst();

  while (length--) {
    temp_last = temp;
    temp = SPI.transfer(0x00);
    //Read JPEG data from FIFO
    if ((temp == 0xD9) && (temp_last == 0xFF)) { //If find the end, break while
      buffer[i++] = temp;  //save the last  0XD9
      //Write the remain bytes in the buffer
      cam->CS_HIGH();

      file.write(buffer, i);
      file.close();
      Serial.println("Image saved");

      is_header = false;
      i = 0;
    }

    if (is_header) {
       //Write image data to buffer if not full
      if (i < BUFFER_SIZE) {
        buffer[i++] = temp;
      } else {
        //Write 256 bytes image data to client
        cam->CS_HIGH();
        file.write(buffer, BUFFER_SIZE);

        i = 0;
        buffer[i++] = temp;

        cam->CS_LOW();
        cam->set_fifo_burst();
      }
    } else if ((temp == 0xD8) & (temp_last == 0xFF)) {
      is_header = true;
      buffer[i++] = temp_last;
      buffer[i++] = temp;
    }
  }

  return filename;
}
