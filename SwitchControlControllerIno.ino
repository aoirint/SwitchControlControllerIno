#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "NoReportIDHID.h"

static const uint8_t _hidReportDescriptor[] PROGMEM = {
    0x05, 0x01,       //   USAGE_PAGE (Generic Desktop)
    0x09, 0x05,       //   USAGE (Game Pad)
    0xa1, 0x01,       //   COLLECTION (Application)
    0x15, 0x00,       //   LOGICAL_MINIMUM (0)
    0x25, 0x01,       //   LOGICAL_MAXIMUM (1)
    0x35, 0x00,       //   PHYSICAL_MINIMUM (0)
    0x45, 0x01,       //   PHYSICAL_MAXIMUM (1)
    0x75, 0x01,       //   REPORT_SIZE (1)
    0x95, 0x10,       //   REPORT_COUNT (16)
    0x05, 0x09,       //   USAGE_PAGE (Button)
    0x19, 0x01,       //   USAGE_MINIMUM (1)
    0x29, 0x10,       //   USAGE_MAXIMUM (16)
    0x81, 0x02,       //   INPUT (Data,Var,Abs)
    0x05, 0x01,       //   USAGE_PAGE (Generic Desktop)
    0x25, 0x07,       //   LOGICAL_MAXIMUM (7)
    0x46, 0x3b, 0x01, //   PHYSICAL_MAXIMUM (315)
    0x75, 0x04,       //   REPORT_SIZE (4)
    0x95, 0x01,       //   REPORT_COUNT (1)
    0x65, 0x14,       //   UNIT (20)
    0x09, 0x39,       //   USAGE (Hat Switch)
    0x81, 0x42,       //   INPUT (Data,Var,Abs)
    0x65, 0x00,       //   UNIT (0)
    0x95, 0x01,       //   REPORT_COUNT (1)
    0x81, 0x01,       //   INPUT (Cnst,Arr,Abs)
    0x26, 0xff, 0x00, //   LOGICAL_MAXIMUM (255)
    0x46, 0xff, 0x00, //   PHYSICAL_MAXIMUM (255)
    0x09, 0x30,       //   USAGE (X)
    0x09, 0x31,       //   USAGE (Y)
    0x09, 0x32,       //   USAGE (Z)
    0x09, 0x35,       //   USAGE (Rz)
    0x75, 0x08,       //   REPORT_SIZE (8)
    0x95, 0x04,       //   REPORT_COUNT (4)
    0x81, 0x02,       //   INPUT (Data,Var,Abs)
    0x06, 0x00, 0xff, //   USAGE_PAGE (Vendor Defined 65280)
    0x09, 0x20,       //   USAGE (32)
    0x95, 0x01,       //   REPORT_COUNT (1)
    0x81, 0x02,       //   INPUT (Data,Var,Abs)
    0x0a, 0x21, 0x26, //   USAGE (9761)
    0x95, 0x08,       //   REPORT_COUNT (8)
    0x91, 0x02,       //   OUTPUT (Data,Var,Abs)
    0xc0              // END_COLLECTION
};

typedef struct
{
	uint16_t button;
	uint8_t hat;
	uint8_t leftX;
	uint8_t leftY;
	uint8_t rightX;
	uint8_t rightY;
	uint8_t vendorSpec;
} USB_JoystickReport_Input_t;

const uint8_t STICK_CENTER = 0x7F;
const uint8_t HAT_CENTER = 0x08;


SoftwareSerial serialToProxy(2, 3); // RX, TX
USB_JoystickReport_Input_t joystickData;

void setup() {
  static HIDSubDescriptor node(_hidReportDescriptor, sizeof(_hidReportDescriptor));
  NoReportIDHID.AppendDescriptor(&node);

  memset(&joystickData, 0, sizeof(USB_JoystickReport_Input_t));
  joystickData.hat = HAT_CENTER;
  joystickData.leftX = STICK_CENTER;
  joystickData.leftY = STICK_CENTER;
  joystickData.rightX = STICK_CENTER;
  joystickData.rightY = STICK_CENTER;

  serialToProxy.begin(115200);
}

void loop() {
  if (serialToProxy.available()) {
    serialToProxyEvent();
  }
}

void serialToProxyEvent() {
  StaticJsonDocument<255> msg;
  bool jsonError = false;

  if (nextSerialJson(serialToProxy, &msg, &jsonError)) {
    joystickData.button = msg["button"];
    joystickData.hat = msg["hat"];
    joystickData.leftX = msg["leftX"];
    joystickData.leftY = msg["leftY"];
    joystickData.rightX = msg["rightX"];
    joystickData.rightY = msg["rightY"];

    NoReportIDHID.SendReport(&joystickData, sizeof(USB_JoystickReport_Input_t));
  }
  else if (jsonError) {
    // StaticJsonDocument<255> response;
    // serializeJson(response, Serial); Serial.println();
  }
}
