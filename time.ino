/*
|  This file is a part of the Morning Buddy project.
|  It contains the components that control/assess the time during the runtime.
*/


unsigned long currentTimeRequest() {
  WiFiClient client;
  HTTPClient http;
  String payload;

  while (true) {
    http.begin(client, "http://worldtimeapi.org/api/timezone/Europe/London.json");

    int httpCode = http.GET();

    if (httpCode == HTTP_CODE_OK) {
      payload = http.getString();
      break;
    } else {
      Serial.print("HTTP error code: ");
      WebSerial.print("HTTP error code: ");
      Serial.println(httpCode);
      WebSerial.println(httpCode);
      delay(2000);
    }

    http.end();
  }

  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print("Failed to parse JSON: ");
    WebSerial.print("Failed to parse JSON: ");
    Serial.println(error.c_str());
    WebSerial.println(error.c_str());
  } else {

    return doc["unixtime"];
  }

  return 0;
}

void syncTime() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }
  webTime = currentTimeRequest();
  lastSyncTime = webTime;
  lastMillis = millis();
}

void findNextAlarm() {
  unsigned long n = alarmUNIXbase + alarmTime[0] * 3600 + alarmTime[1] * 60;
  if (lastAlarm == 0) {
    while (n < currentTime) {
      n += secondsPerDay;
      delay(5);
    }
  } else {
    n = lastAlarm + secondsPerDay;
  }
  nextAlarm = n;
  Serial.print("Next alarm: ");
  WebSerial.print("Next alarm: ");
  Serial.println(nextAlarm);
  WebSerial.println(nextAlarm);
}

void updateCurrentTime() {
  currentTime = webTime + (millis() - lastMillis) / 1000;
}

void checkSync() {
  if (currentTime - lastSyncTime >= syncInterval) syncTime();
}

bool isoverdue() {
  updateCurrentTime();
  if (currentTime >= nextAlarm) {
    if (allChallengesCompleted) return false;
    return true;
  }
  return false;
}

void checkAlarm() {
  if ((nextAlarm - timeWindow) <= currentTime && currentTime < nextAlarm) {
    led(true);
    btn.tick();
    if (btn.click()) {
      blinker(32, 25);
      bool challengesCompleted[] = { 0, 0, 0 };
      if (!isoverdue()) challengesCompleted[0] = UnlockTheSafe();
      if (!isoverdue()) challengesCompleted[1] = RepeatThePattern();
      if (!isoverdue()) challengesCompleted[2] = Mathematics();
      if (challengesCompleted[0] && challengesCompleted[1] && challengesCompleted[2]) allChallengesCompleted = true;
      if (isoverdue()) {
        sendTGMsg(false);
        blinker(8, 200);
      } else {
        sendTGMsg(true);
        blinker(32, 25);
      }
      lastAlarm = nextAlarm;
      findNextAlarm();
      allChallengesCompleted = false;
    }
  } else {
    led(false);
  }
  if (isoverdue()) {
    sendTGMsg(false);
    blinker(8, 200);
    lastAlarm = nextAlarm;
    findNextAlarm();
  }
}
