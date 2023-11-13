/*
|  This file is a part of the Morning Buddy project.
|  It contains the startup function.
*/


void startup () {
  Serial.begin(115200);
  delay(10);
  pinMode(LED_BUILTIN, OUTPUT);
  led(false);
  led(true);
  if (winterTime) alarmUNIXbase += 3600; // a winter time 1-hour adjustment if needed
  connectToWiFi();
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
  WebSerial.begin(&server);
  WebSerial.msgCallback(recvMsg);
  server.begin();
  OTADRIVE.setInfo("myinfo", "v@1.0.11");
  syncTime();
  updateCurrentTime();
  findNextAlarm();
  led(false);
}
