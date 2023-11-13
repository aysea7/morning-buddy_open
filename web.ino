/*
|  This file is a part of the Morning Buddy project.
|  It contains the internet-related components that are not related to time processes.
*/


void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void ota() {
  if (OTADRIVE.timeTick(900)) OTADRIVE.updateFirmware();
}

void sendTGMsg(bool success) {
  String data;
  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient https;
  if (success) {
    data = "вже прокинувся.";
  } else {
    data = "досі сплю.";
  }
  if (https.begin(client, "https://api.telegram.org/botTOKENHERE/sendMessage?chat_id=CHATIDHERE&text=" + data)) {
    int httpCode = https.GET();
    Serial.println("============== Response code: " + String(httpCode));
    WebSerial.println("============== Response code: " + String(httpCode));
    if (httpCode > 0) {
      Serial.println(https.getString());
      WebSerial.println(https.getString());
    }
    https.end();
  } else {
    Serial.printf("[HTTPS] Unable to connect\n");
    WebSerial.printf("[HTTPS] Unable to connect\n");
  }
}

void recvMsg(uint8_t *data, size_t len){
  WebSerial.println("Received Data...");
  String d = "";
  for(int i=0; i < len; i++){
    d += char(data[i]);
  }
  WebSerial.println(d);
}
