#include <Arduino_GFX_Library.h>
#include "SD.h"
#include "SPI.h"

#include "ft6336u.h"
#define SPI_MISO 42
#define SPI_MOSI 2
#define SPI_SCLK 1

#define SD_CS 38

#define LCD_CS 39
#define LCD_DC 41
#define LCD_RST 40
#define LCD_BL 6

#define TP_INT 17
#define TP_RST 16
#define TP_SDA 15
#define TP_SCL 7


#define LCD_VER_RES 480
#define LCD_HOR_RES 320


SPIClass fspi(FSPI);

Arduino_DataBus* bus = new Arduino_ESP32SPI(LCD_DC /* DC */, LCD_CS /* CS */, SPI_SCLK /* SCK */, SPI_MOSI /* MOSI */, SPI_MISO /* MISO */, FSPI);
Arduino_GFX* gfx = new Arduino_ST7796(
  bus, LCD_RST /* RST */, 0 /* rotation */, true, LCD_HOR_RES, LCD_VER_RES);


void display_color_test(void) {
  gfx->fillScreen(WHITE);
  delay(1000);  // 1 seconds
  gfx->fillScreen(RED);
  delay(1000);  // 1 seconds
  gfx->fillScreen(GREEN);
  delay(1000);  // 1 seconds
  gfx->fillScreen(BLUE);
  delay(1000);  // 1 secondsb
  gfx->fillScreen(WHITE);
}


uint64_t cardSize;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Arduino_GFX Hello World example");

  touch_init(TP_SDA, TP_SCL, TP_RST, TP_INT);

  // Init Display
  if (!gfx->begin()) {
    Serial.println("gfx->begin() failed!");
  }
  pinMode(LCD_BL, OUTPUT);
  digitalWrite(LCD_BL, HIGH);

  // sd卡的初始化要放到lcd的后面
  fspi.begin(SPI_SCLK, SPI_MISO, SPI_MOSI, -1);  //SCLK, MISO, MOSI, SS
  if (!SD.begin(SD_CS, fspi)) {
    Serial.println("Card Mount Failed");
  } else {
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE) {
      Serial.println("No SD card attached");
      return;
    }
    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC) {
      Serial.println("MMC");
    } else if (cardType == CARD_SD) {
      Serial.println("SDSC");
    } else if (cardType == CARD_SDHC) {
      Serial.println("SDHC");
    } else {
      Serial.println("UNKNOWN");
    }
    cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
  }

  display_color_test();
}
toucht_coords_t toucht_coords;
uint16_t sd_count = 0;
void loop() {
  if (get_touch_coords(&toucht_coords))
  {
    gfx->drawCircle(toucht_coords.x, toucht_coords.y, 2, RED);
  }
  if (sd_count++ > 200) {
    sd_count = 0;
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
  }
  delay(10);
}
