/*
|  This file is a part of the Morning Buddy project.
|  It contains the LED-related components.
*/


void led(bool on) {
  if (on) {
    digitalWrite(LED_BUILTIN, LOW);
  } else {
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

void blinker(int count, int holdFor) {
  for (uint8_t i = 0; i < count; i++) {
    led(true);
    delay(holdFor);
    led(false);
    delay(holdFor);
  }
}