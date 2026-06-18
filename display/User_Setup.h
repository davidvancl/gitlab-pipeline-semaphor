// ============================================================
// User_Setup.h pro TFT_eSPI
// Waveshare 1.69" LCD (ST7789V2) + NodeMCU V2 ESP8266
//
// UMÍSTĚNÍ: přepis souboru User_Setup.h uvnitř složky
//           Arduino/libraries/TFT_eSPI/User_Setup.h
// ============================================================

// -- Driver chip --
#define ST7789_DRIVER

// -- Rozlišení displeje --
#define TFT_WIDTH  240
#define TFT_HEIGHT 280

// -- SPI piny (NodeMCU V2 ESP8266) --
#define TFT_MOSI   13   // D7  – DIN
#define TFT_SCLK   14   // D5  – CLK
#define TFT_CS     12   // D6  – CS (bylo D8/GPIO15, blokoval boot!)
#define TFT_DC      4   // D2  – DC
#define TFT_RST     5   // D1  – RST
#define TFT_BL      2   // D4  – BL (podsvícení)

#define TFT_BACKLIGHT_ON HIGH

// -- SPI rychlost --
#define SPI_FREQUENCY       40000000
#define SPI_READ_FREQUENCY   6000000

// -- Fonty (volitelné, pro vyšší kvalitu textu) --
#define LOAD_GLCD   // základní 5x7 font
#define LOAD_FONT2  // malý font
#define LOAD_FONT4  // střední font
#define LOAD_FONT6  // velká čísla
#define LOAD_FONT7  // 7-segmentový styl
#define LOAD_GFXFF  // FreeFont podpora
#define SMOOTH_FONT