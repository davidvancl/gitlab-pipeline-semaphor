#define ST7789_DRIVER

#define TFT_WIDTH  240
#define TFT_HEIGHT 280

#define TFT_MOSI   13   // D7  – DIN
#define TFT_SCLK   14   // D5  – CLK
#define TFT_CS     12   // D6  – CS
#define TFT_DC      4   // D2  – DC
#define TFT_RST     5   // D1  – RST
#define TFT_BL      2   // D4  – BL

#define TFT_BACKLIGHT_ON HIGH

#define SPI_FREQUENCY       40000000
#define SPI_READ_FREQUENCY   6000000

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_GFXFF
#define SMOOTH_FONT