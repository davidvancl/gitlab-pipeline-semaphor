#include "arduino_secrets.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define API_URL "https://gitlab.wpj.cz/api/v4/projects/1/pipelines/latest"
#define REQUEST_DELAY 5000
#define REQUEST_TIMEOUT 20000
#define RED_LED 14
#define GREEN_LED 13
#define BUZZER 12

const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;

long lastUpdate = 0;

void setup() {
  Serial.begin(115200);

  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, LOW);

  pinMode(GREEN_LED, OUTPUT);
  digitalWrite(GREEN_LED, LOW);

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER, LOW);

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
        processPayload(http);
      }
    } else {
      Serial.printf("[HTTP] GET... selhalo, chyba: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  lastUpdate = millis();
}

void processPayload(HTTPClient &http) {
  JsonDocument document;
  DeserializationError error = deserializeJson(document, http.getStream());

  if (error) {
    Serial.print("deserializeJson() returned ");
    Serial.println(error.c_str());
    return;
  }

  const char* status = document["status"];
  
  if (strcmp(status, "success") == 0) {
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    digitalWrite(BUZZER, LOW);
  } else {
    digitalWrite(GREEN_LED, LOW);
    digitalWrite(RED_LED, HIGH);
    digitalWrite(BUZZER, HIGH);
  }

  Serial.print("Gitlab pipeline response: ");
  serializeJson(document, Serial);
  Serial.println();
}
