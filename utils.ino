/*
|  This file is a part of the Morning Buddy project.
|  It contains the utilitary components.
*/


void checkEncoder(int8_t left, int8_t right) {
  int8_t v = enc.tick(digitalRead(PIN_A), digitalRead(PIN_B));
  if (v) {
    if ((encoderPosition + v) != (left - 1) && (encoderPosition + v) != (right + 1)) {
      encoderPosition += v;
      Serial.println(encoderPosition);
      WebSerial.println(encoderPosition);
    }
  }
}
