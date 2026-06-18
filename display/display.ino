#include <TFT_eSPI.h>
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();

void setup() {
  Serial.begin(115200);

  // Podsvícení zapnout
  pinMode(2, OUTPUT);
  digitalWrite(2, HIGH);

  // Inicializace displeje
  tft.init();
  tft.setRotation(0);          // Portrét, 0–3 dle potřeby

  // Černé pozadí
  tft.fillScreen(TFT_BLACK);

  // ── Nadpis ──────────────────────────────────────────────────
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(3);
  tft.setTextDatum(MC_DATUM);  // střed vodorovně i svisle
  tft.drawString("Hello", 120, 110);

  // ── Druhý řádek ─────────────────────────────────────────────
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(3);
  tft.drawString("World!", 120, 155);

  // ── Tenká dekorační čára ─────────────────────────────────────
  tft.drawFastHLine(30, 135, 180, TFT_DARKGREY);

  // ── Menší info text dole ─────────────────────────────────────
  tft.setTextColor(TFT_DARKGREY, TFT_BLACK);
  tft.setTextSize(1);
  tft.drawString("ESP8266 + ST7789V2", 120, 240);

  Serial.println("Hotovo!");
}

void loop() {
  // Nic – statický obsah
}
