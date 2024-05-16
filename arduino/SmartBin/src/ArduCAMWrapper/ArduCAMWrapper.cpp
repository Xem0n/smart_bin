#include <Wire.h>
#include <SPI.h>
#include <ArduCAM.h>

#include "ArduCAMWrapper.h"

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
  delay(1000);
  myCAM.clear_fifo_flag();

  return myCAM;
}

SmartBin::ArduCAMWrapper::Image SmartBin::ArduCAMWrapper::captureImage(ArduCAM* cam) {
  SmartBin::ArduCAMWrapper::Image image;

  cam->flush_fifo();
  cam->clear_fifo_flag();
  cam->start_capture();

  while(!cam->get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK));

  byte* buffer = new byte[256];
  size_t length = cam->read_fifo_length();
  size_t initLength = length;
  size_t offset = 0;
  size_t i = 0;
  uint8_t temp = 0, temp_last = 0;
  bool is_header = false;
  image.data = new byte[length];
  image.length = length;

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
      if (i < 256) {
        buffer[i++] = temp;
      } else {
        //Write 256 bytes image data to client
        cam->CS_HIGH();
        memcpy(image.data + offset, buffer, 256);
        offset += 256;
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

  delete[] buffer;

  return image;
}