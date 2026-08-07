#include "arduino_secrets.h"
#include "display.h"
#include "config.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

TFT_eSPI tft = TFT_eSPI();
const char* ssid = SECRET_SSID;
const char* password = SECRET_PASS;
long lastUpdate = 0;

void setup() {
  Serial.begin(115200);
  
  // Font loader
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }
  Serial.println("\r\nSPIFFS available!");

  // Check font availability
  bool font_missing = false;
  if (SPIFFS.exists("/Charis_SILR.vlw")    == false) font_missing = true;

  if (font_missing)
  {
    Serial.println("\r\nFont missing in SPIFFS, did you upload it?");
    while(1) yield();
  }
  else Serial.println("\r\nFonts found OK.");

  // Init required pins
  pinMode(RED_RELAY_GPIO, OUTPUT);
  pinMode(YELLOW_RELAY_GPIO, OUTPUT);
  pinMode(GREEN_RELAY_GPIO, OUTPUT);
  pinMode(DISPLAY_BACKGROUND_LIGHT_GPIO, OUTPUT);

  // Setup yellow light as init
  digitalWrite(RED_RELAY_GPIO, LOW);
  digitalWrite(YELLOW_RELAY_GPIO, HIGH);
  digitalWrite(GREEN_RELAY_GPIO, LOW);

  // Init display
  digitalWrite(DISPLAY_BACKGROUND_LIGHT_GPIO, HIGH);
  tft.init();
  tft.setRotation(0);

  // Render init frame
  display(tft, "WELCOME", "David Vancl");

  // Setup Wi-Fi and establish connection to git
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("\nConnecting");
  while(WiFi.status() != WL_CONNECTED){
      Serial.print(".");
      delay(100);
  }

  Serial.println("\nConnected to the WiFi network");
  Serial.print("Local IP:");
  Serial.println(WiFi.localIP());
}

void processPayload(HTTPClient &http) {
  JsonDocument document;
  DeserializationError error = deserializeJson(document, http.getStream());

  if (error) {
    // On error turn off semaphore
    digitalWrite(RED_RELAY_GPIO, LOW);
    digitalWrite(YELLOW_RELAY_GPIO, LOW);
    digitalWrite(GREEN_RELAY_GPIO, LOW);

    Serial.print("deserializeJson() returned ");
    Serial.println(error.c_str());
    return;
  }

  const char* status = document["status"];
  const char* author = document["user"]["name"];
  if (
    (status == nullptr) ||
    (strcmp(status, "created") == 0) ||
    (strcmp(status, "waiting_for_resource") == 0) || 
    (strcmp(status, "preparing") == 0) || 
    (strcmp(status, "canceled") == 0) || 
    (strcmp(status, "skipped") == 0)
    ) {
    Serial.print("[RESULT ACTION]: do nothing"); 
  } else if (strcmp(status, "running") == 0 || strcmp(status, "pending") == 0 || strcmp(status, "scheduled") == 0 || strcmp(status, "manual") == 0) {
    digitalWrite(YELLOW_RELAY_GPIO, HIGH);
    display(tft, "WAITING", author);
    Serial.print("[RESULT ACTION]: yellow light on"); 
  } else if (strcmp(status, "success") == 0) {
    digitalWrite(RED_RELAY_GPIO, LOW);
    digitalWrite(YELLOW_RELAY_GPIO, LOW);
    digitalWrite(GREEN_RELAY_GPIO, HIGH);
    display(tft, "SUCCESS", author);
    Serial.print("[RESULT ACTION]: green light on"); 
  } else if (strcmp(status, "failed") == 0) {
    digitalWrite(RED_RELAY_GPIO, HIGH);
    digitalWrite(YELLOW_RELAY_GPIO, LOW);
    digitalWrite(GREEN_RELAY_GPIO, LOW);
    display(tft, "FAILED", author);
    Serial.print("[RESULT ACTION]: red light on"); 
  }

  Serial.print("Gitlab pipeline response: ");
  serializeJson(document, Serial);
  Serial.println();
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
      // On error turn off semaphore
      digitalWrite(RED_RELAY_GPIO, LOW);
      digitalWrite(YELLOW_RELAY_GPIO, LOW);
      digitalWrite(GREEN_RELAY_GPIO, LOW);
      Serial.printf("[HTTP] GET... selhalo, chyba: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  lastUpdate = millis();
}