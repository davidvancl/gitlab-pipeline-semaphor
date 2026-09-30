#include <Arduino.h>
#include <OtaUpdater.h>
#include "Display.h"
#include "Pipeline.h"
#include "Semaphore.h"
#include "TokenStore.h"
#include "WifiCredentials.h"

void setup() {
  Serial.begin(115200);

  Semaphore::begin();
  Semaphore::yellow();

  Display::begin();
  Display::show("WELCOME", "David Vancl");

  Serial.print("Firmware version: ");
  Serial.println(FW_VERSION);
  OtaUpdater::run(WIFI_CREDENTIALS);

#ifdef HAS_SECRETS
  TokenStore::save(SECRET_GITLAB_TOKEN);
#endif
  TokenStore::load();

  Pipeline::begin();
}

void loop() {
  Pipeline::poll();
}
