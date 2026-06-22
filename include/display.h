#pragma once

#include <TFT_eSPI.h>
#include <SPI.h>

void display(TFT_eSPI tft, const char* title, const char* subtitle) {
  tft.fillScreen(TFT_BLACK);

  // Main title
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(3);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(title, 120, 110);

  // Sub title
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);
  tft.drawString(subtitle, 120, 155);

  // Middle line
  tft.drawFastHLine(30, 135, 180, TFT_DARKGREY);

  // Footer text
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("WPJ", 120, 240);
}