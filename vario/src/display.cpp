#include <display.h>

// Use software SPI (slower)
//TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED);
// Use hardware SPI (library has been modified in TFT_22_ILI8225.cpp l311)
TFT_22_ILI9225 tft = TFT_22_ILI9225(TFT_RST, TFT_RS, TFT_CS, TFT_SDI, TFT_CLK, TFT_LED);

void setupDisplay(int orientation, uint8_t * font) {
    tft.begin();
    tft.setOrientation(orientation);
    tft.setFont(font);
}

void clearDisplay() { 
    tft.clear();
}

void displayStr(uint16_t x, uint16_t y, String msg) { 
    tft.drawText(x, y, msg);
}

void displayStatusBar() {
    tft.fillRectangle(0, 0, tft.maxX()-1, 25, COLOR_WHITE);
    tft.fillRectangle(1, 1, tft.maxX()-2, 24, COLOR_DARKGRAY);
    tft.drawBitmap(5, 2, up_right_arrow, 25, 25, COLOR_WHITE);
    tft.drawText((tft.maxX()/2)-25, 5, "17:24");
    tft.drawBitmap(190-25, 2, sd_card, 25, 25, COLOR_WHITE);
    tft.drawBitmap(190, 2, battery, 24, 24, COLOR_WHITE);
}

void displayMenu(int state) {
    //displayStatusBar();
    tft.drawCircle(25, tft.maxY()/2, 22, COLOR_LIGHTCYAN);
    tft.drawCircle(tft.maxX()-25, tft.maxY()/2, 22, COLOR_LIGHTCYAN);
    tft.drawCircle(tft.maxX()/2, tft.maxY()/2, 56, COLOR_WHITE);

    switch (state)
    {
    case 0: // VARIO
        tft.drawBitmap((tft.maxX()/2)-(96/2), (tft.maxY()/2)-(96/2), barometer_big, 96, 96, COLOR_DARKCYAN);
        tft.drawText((tft.maxX()/2)-(30), 160, "VARIO");
        break;
    case 1: // COMPASS
        tft.drawBitmap((tft.maxX()/2)-(96/2), (tft.maxY()/2)-(96/2), compass_big, 96, 96, COLOR_BROWN);
        tft.drawText((tft.maxX()/2)-(40), 160, "COMPASS");
        break;
    case 2: // GPS
        tft.drawBitmap((tft.maxX()/2)-(96/2), (tft.maxY()/2)-(96/2), gps_big, 96, 96, COLOR_RED);
        tft.drawText((tft.maxX()/2)-(30), 160, "GPS");
        break;
    case 3: // ALL
        break;
    case 4: // SETTINGS
        tft.drawBitmap((tft.maxX()/2)-(96/2), (tft.maxY()/2)-(96/2), settings_big, 96, 96, COLOR_DARKGRAY);
        tft.drawText((tft.maxX()/2)-(45), 160, "SETTINGS");
    }

    // Display right item
    int next_state = state + 1;
    if (next_state > 4) next_state = 0;
    switch (next_state)
    {
    case 0: // VARIO
        tft.drawBitmap(tft.maxX()-25-(36/2), (tft.maxY()/2)-(36/2), barometer_small, 36, 36, COLOR_DARKCYAN);
        break;
    case 1: // COMPASS
        tft.drawBitmap(tft.maxX()-25-(36/2), (tft.maxY()/2)-(36/2), compass_small, 36, 36, COLOR_BROWN);
        break;
    case 2: // GPS
        tft.drawBitmap(tft.maxX()-25-(36/2), (tft.maxY()/2)-(36/2), gps_small, 36, 36, COLOR_RED);

        break;
    case 3: // ALL
        break;
    case 4: // SETTINGS
        tft.drawBitmap(tft.maxX()-25-(36/2), (tft.maxY()/2)-(36/2), settings_small, 36, 36, COLOR_DARKGRAY);
    }

    // Display right item
    next_state = state - 1;
    if (next_state < 0) next_state = 4;
    switch (next_state)
    {
    case 0: // VARIO
        tft.drawBitmap(25-(36/2), (tft.maxY()/2)-(36/2), barometer_small, 36, 36, COLOR_DARKCYAN);
        break;
    case 1: // COMPASS
        tft.drawBitmap(25-(36/2), (tft.maxY()/2)-(36/2), compass_small, 36, 36, COLOR_BROWN);
        break;
    case 2: // GPS
        tft.drawBitmap(25-(36/2), (tft.maxY()/2)-(36/2), gps_small, 36, 36, COLOR_RED);
        break;
    case 3: // ALL
        break;
    case 4: // SETTINGS
        tft.drawBitmap(25-(36/2), (tft.maxY()/2)-(36/2), settings_small, 36, 36, COLOR_DARKGRAY);
        break;
    }

}

/* ############# VARIO ############# */
void displayVarioPage(){

    // Ascent rate, middle  
    displayStr(170, 50, "m/s");

    // Temperature, lower middle
    tft.drawBitmap(60, (tft.maxY()/2)+15, thermometer_small, 25, 25, COLOR_WHITE);
    displayStr(150, (tft.maxY()/2)+20, "C");
    
    // Pressure, left of the screen
    tft.drawBitmap(30, tft.maxY()-48, barometer_small, 25, 25, COLOR_WHITE);
    displayStr(65, 155, "hPa");

    // Altitude, right of the screen
    tft.drawBitmap(tft.maxX()-58, tft.maxY()-48, mountain, 25, 25, COLOR_WHITE);
    
    displayStr(200, 155, "m");
}

void displayVarioInfos(float altitude, float pressure, float temperature, float ascent_rate) {
    tft.drawRectangle(0, 100, tft.maxX()-1, tft.maxY()-1, COLOR_GRAY);

    tft.drawRectangle(0, 25, tft.maxX()-1, 99, COLOR_WHITE);
    // Ascent rate
    // Draw rectangle before to avoid seeing the bitmap below
    tft.fillRectangle(15, 35, 15+48, 45+48, COLOR_BLACK);
    if (ascent_rate > 0) tft.drawBitmap(15, 35, ascend, 48, 48, COLOR_GREEN);
    else tft.drawBitmap(15, 35, descend, 48, 48, COLOR_RED);
      
    displayStr(90, STATUS_BAR_OFFSET_Y+15, String(ascent_rate, 1));

    // Temp
    displayStr(90, (tft.maxY()/2)+20, String(temperature, 1));

    // Pressure, temp, altitude
    displayStr(130, STATUS_BAR_OFFSET_Y+120, String((int)altitude));
    displayStr(10, STATUS_BAR_OFFSET_Y+120, String((int)pressure));
}