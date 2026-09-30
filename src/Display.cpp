#include "Display.h"

#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include "config.h"

namespace Display {

static const char* FONT_NAME = "Charis_SILR";

static TFT_eSPI tft = TFT_eSPI();

void begin() {
  pinMode(DISPLAY_BACKGROUND_LIGHT_GPIO, OUTPUT);
  digitalWrite(DISPLAY_BACKGROUND_LIGHT_GPIO, HIGH);

  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialisation failed!");
    while (1) yield();
  }

  if (!SPIFFS.exists(String("/") + FONT_NAME + ".vlw")) {
    Serial.println("Font missing in SPIFFS, did you upload it?");
    while (1) yield();
  }

  tft.init();
  tft.setRotation(0);
}

void show(const char* title, const char* subtitle) {
  tft.fillScreen(TFT_BLACK);
  tft.loadFont(FONT_NAME);

  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(title, 120, 110);

  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.drawString(subtitle, 120, 155);

  tft.drawFastHLine(30, 135, 180, TFT_DARKGREY);

  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.drawString("WPJ", 120, 240);

  tft.unloadFont();
}

}
