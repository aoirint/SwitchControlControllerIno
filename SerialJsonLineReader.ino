// Requirements:
// #include <ArduinoJson.h>

String serialBuffer = "";

bool nextSerialLine(SoftwareSerial serial, String *serialLine) {
  *serialLine = "";

  while (serial.available() > 0) {
    int ch = serial.read();
    if (ch != -1) {
      if (ch == '\n') {
        *serialLine = serialBuffer;
        serialBuffer = "";
        return true;
      }

      serialBuffer += (char) ch;
    }
  }

  return false;
}

bool nextSerialJson(SoftwareSerial serial, JsonDocument *serialJson, bool *jsonError) {
  String serialLine = "";

  *jsonError = false;
  serialJson->clear();

  if (nextSerialLine(serial, &serialLine)) {
    DeserializationError error;
    error = deserializeJson(*serialJson, serialLine);

    if (error == DeserializationError::Ok) {
      return true;
    }

    *jsonError = true;
    serialJson->clear();
  }

  return false;
}
