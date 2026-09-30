#include "Pipeline.h"

#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include "Display.h"
#include "Semaphore.h"
#include "TokenStore.h"
#include "config.h"

namespace Pipeline {

static unsigned long lastUpdate = 0;
static String lastTitle;
static String lastAuthor;

static void showIfChanged(const char* title, const char* author) {
  if (lastTitle == title && lastAuthor == author) return;
  lastTitle = title;
  lastAuthor = author;
  Display::show(title, author);
}

static void apply(JsonDocument& document) {
  const char* status = document["status"];
  const char* author = document["user"]["name"];
  if (author == nullptr) author = "";

  if (
    status == nullptr ||
    strcmp(status, "created") == 0 ||
    strcmp(status, "waiting_for_resource") == 0 ||
    strcmp(status, "preparing") == 0 ||
    strcmp(status, "canceled") == 0 ||
    strcmp(status, "skipped") == 0
  ) {
    return;
  }

  if (strcmp(status, "running") == 0 || strcmp(status, "pending") == 0 ||
      strcmp(status, "scheduled") == 0 || strcmp(status, "manual") == 0) {
    Semaphore::yellow();
    showIfChanged("WAITING", author);
  } else if (strcmp(status, "success") == 0) {
    Semaphore::green();
    showIfChanged("SUCCESS", author);
  } else if (strcmp(status, "failed") == 0) {
    Semaphore::red();
    showIfChanged("FAILED", author);
  }
}

static void fetch() {
  WiFiClientSecure client;
  client.setInsecure();

  HTTPClient http;
  http.setTimeout(REQUEST_TIMEOUT);

  if (!http.begin(client, API_URL)) return;

  http.addHeader("PRIVATE-TOKEN", TokenStore::gitlab());
  int httpCode = http.GET();

  if (httpCode > 0) {
    Serial.printf("[Pipeline] GET returned %d\n", httpCode);
    if (httpCode == HTTP_CODE_OK) {
      JsonDocument document;
      DeserializationError error = deserializeJson(document, http.getStream());
      if (error) {
        Serial.print("[Pipeline] deserializeJson() returned ");
        Serial.println(error.c_str());
        Semaphore::off();
      } else {
        apply(document);
      }
    }
  } else {
    Semaphore::off();
    Serial.printf("[Pipeline] GET failed: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}

void begin() {
  lastUpdate = millis() - REQUEST_DELAY;
}

void poll() {
  if (millis() - lastUpdate < REQUEST_DELAY) return;
  if (WiFi.status() != WL_CONNECTED) return;

  lastUpdate = millis();
  fetch();
}

}
