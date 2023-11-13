/*
|  This file is a part of the Morning Buddy project.
|  It contains the challenges one has to complete in order to get a positive result.
*/


bool UnlockTheSafe() {
  led(false);

  int8_t lockSlots[3];

  WebSerial.println("Generating slots...");
  // generate slots that need to be opened
  while (true) {
    for (uint8_t i = 0; i < 3; i++) {
      lockSlots[i] = random(-50, 50);
    }
    if (lockSlots[0] != lockSlots[1] && lockSlots[0] != lockSlots[2] && lockSlots[1] != lockSlots[2]) break;
    delay(5);
  }
  WebSerial.print("Slots: ");
  WebSerial.print(lockSlots[0]);
  WebSerial.print(", ");
  WebSerial.print(lockSlots[1]);
  WebSerial.print(", ");
  WebSerial.print(lockSlots[2]);

  delay(25);

  // game itself
  for (uint8_t i = 0; i < 3; i++) {
    Serial.println(lockSlots[i]);
    WebSerial.print("Current slot: ");
    WebSerial.println(lockSlots[i]);
    while (true) {
      if (isoverdue()) break;
      checkEncoder(-50, 50);
      if (encoderPosition == lockSlots[i]) {
        led(true);
      } else {
        led(false);
      }
      btn.tick();
      if (btn.click()) {
        if (encoderPosition == lockSlots[i]) {
          encoderPosition = 0;
          blinker(20, 75);
          break;
        }
      }
      delay(5);
    }
  }
  if (isoverdue()) return false;
  blinker(8, 150);
  Serial.println("Safe unlocked successfully!");
  WebSerial.println("Safe unlocked successfully!");
  return true;
}

bool RepeatThePattern() {
  return true;
}
bool Mathematics() {
  return true;
}