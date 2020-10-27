#include <Arduino.h>

#include "SPI.h"
#include "TFT_22_ILI9225.h"

#define TFT_RST 4
#define TFT_RS  19
#define TFT_CS  5  // CS
#define TFT_SDI 23  // MOSI
#define TFT_CLK 18  // SCK
#define TFT_LED 0   // 0 if wired to +5V directly

/*
Hardware hookup

LCD Pin                 Arduino Pin
-------                 -----------
 1  Vcc                 Vcc
 2  Gnd                 Gnd
 3  Gnd                 Gnd or N/C
 4                      N/C
 5                      N/C
 6  LED (backlight)     D3         -> not available on my LCD ?
 7  CLK                 GPIO18 (SCK)
 8  SDI                 GPIO23 (MOSI)
 9  RS                  GPIO19
10  RST                 GPIO4
11  CS                  GPIO5
*/

// Use hardware SPI (faster - on Uno: 13-SCK, 12-MISO, 11-MOSI)
//TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_LED);
// Use software SPI (slower)
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED);

void setup() {
    tft.begin();
    Serial.begin(9600);
}

void loop() {
    tft.drawRectangle(0, 0, tft.maxX() - 1, tft.maxY() - 1, COLOR_WHITE);
    tft.setFont(Terminal6x8);
    tft.drawText(10, 10, "ILI9225");
    delay(1000);

    tft.clear();
    tft.drawText(10, 20, "clear");
    delay(1000);

    tft.drawText(10, 30, "Small Font");
    tft.setBackgroundColor(COLOR_YELLOW);
    tft.setFont(Terminal12x16);
    tft.drawText(90, 30, "BIG FONT", COLOR_RED);
    tft.setBackgroundColor(COLOR_BLACK);
    tft.setFont(Terminal6x8);
    delay(1000);

    tft.drawText(10, 40, "setBacklight off");
    delay(500);
    tft.setBacklight(LOW);
    delay(500);
    tft.setBacklight(HIGH);
    tft.drawText(10, 50, "setBacklight on");
    delay(1000);

    tft.drawRectangle(10, 10, 110, 110, COLOR_BLUE);
    tft.drawText(10, 60, "rectangle");
    delay(1000);

    tft.fillRectangle(20, 20, 120, 120, COLOR_RED);
    tft.drawText(10, 70, "solidRectangle");
    delay(1000);

    tft.drawCircle(80, 80, 50, COLOR_YELLOW);
    tft.drawText(10, 80, "circle");
    delay(1000);

    tft.fillCircle(90, 90, 30, COLOR_GREEN);
    tft.drawText(10, 90, "solidCircle");
    delay(1000);

    tft.drawLine(0, 0, tft.maxX() - 1, tft.maxY() - 1, COLOR_CYAN);
    tft.drawText(10, 100, "line");
    delay(1000);

    for (uint8_t i = 0; i < 127; i++)
    tft.drawPixel(random(tft.maxX()), random(tft.maxY()), random(0xffff));
    tft.drawText(10, 110, "point");
    delay(1000);

    for (uint8_t i = 0; i < 4; i++) {
    tft.clear();
    tft.setOrientation(i);
    tft.drawRectangle(0, 0, tft.maxX() - 1, tft.maxY() - 1, COLOR_WHITE);
    tft.drawText(10, 10, "setOrientation (" + String("0123").substring(i, i + 1) + ")");
    tft.drawRectangle(10, 20, 50, 60, COLOR_GREEN);
    tft.drawCircle(70, 80, 10, COLOR_BLUE);
    tft.drawLine(30, 40, 70, 80, COLOR_YELLOW);
    delay(1000);
    }

    tft.setOrientation(0);
    tft.clear();
    tft.setFont(Terminal12x16);
    tft.setBackgroundColor(COLOR_YELLOW);
    tft.drawText(10, 40, "Loony Tunes!", COLOR_RED);
    tft.setBackgroundColor(COLOR_RED);
    tft.setFont(Terminal6x8);
    delay(1000);

    tft.drawText(10, 60, "  That's all folks!   ");

    tft.setFont(Terminal12x16);
    tft.setBackgroundColor(COLOR_BLUE);
    tft.drawText(10, 100, "   ILI9225   ");
    delay(1000);
}