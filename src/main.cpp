#include <Arduino.h>
#include <EEPROM.h>
#include <OtaUpdater.h>
#include "Display.h"
#include "Pipeline.h"
#include "Semaphore.h"
#include "WifiCredentials.h"

static void migrateLegacyToken() {
  struct {
    uint32_t magic;
    char gitlab[96];
  } legacy;
  EEPROM.begin(256);
  EEPROM.get(128, legacy);
  EEPROM.end();
  if (legacy.magic != 0x544F4B32) return;

  legacy.gitlab[sizeof(legacy.gitlab) - 1] = '\0';
  OtaUpdater::saveSecret("gitlab", legacy.gitlab);
  Serial.println("Legacy token migrated.");
}

void setup() {
  Serial.begin(115200);

  Semaphore::begin();
  Semaphore::yellow();

  Display::begin();
  Display::show("WELCOME", "David Vancl");

  Serial.print("Firmware version: ");
  Serial.println(FW_VERSION);
  OtaUpdater::run(WIFI_CREDENTIALS);

  migrateLegacyToken();
#ifdef HAS_SECRETS
  OtaUpdater::saveSecret("gitlab", SECRET_GITLAB_TOKEN);
#endif

  Pipeline::begin();
}

void loop() {
  Pipeline::poll();
}
