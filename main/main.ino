#include "arduino_secrets.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define API_URL "https://gitlab.wpj.cz/api/v4/projects/1/pipelines?ref=master&per_page=1"
#define REQUEST_DELAY 5000
#define REQUEST_TIMEOUT 20000

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

long lastUpdate = 0;

void setup() {
  Serial.begin(115200);

  // WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");
  while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local ESP32 IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if ((millis() - lastUpdate < REQUEST_DELAY) || WiFi.status() != WL_CONNECTED) {
    return;
  }

  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  http.setTimeout(REQUEST_TIMEOUT);

  if (http.begin(client, API_URL)) {
    http.addHeader("PRIVATE-TOKEN", SECRET_GITLAB_TOKEN);
    int httpCode = http.GET();

    if (httpCode > 0) {
      Serial.printf("[HTTP] GET... kód: %d\n", httpCode);
      if (httpCode == HTTP_CODE_OK) {
        String payload = http.getString();
        Serial.println(payload);
      }
    } else {
      Serial.printf("[HTTP] GET... selhalo, chyba: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  lastUpdate = millis();
}
