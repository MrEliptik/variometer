#include <Arduino.h>
#include "SPI.h"

#include <barometer.h>
#include <display.h>

// Need to use GPIO number
//TODO : update these pins with the ESP32
#define GPS_PPS_PIN             0
#define LED_PIN                 3
#define BUZZER_PIN              14
#define SLCK_PIN                5
#define MISO_PIN                6
#define MOSI_PIN                7
#define CS_PIN                  8

#define JOYSTICK_PIN            36

// ######### GENERAL #########
#define LOOP_HERTZ              60                                  // Hz
#define LOOP_TIME               1.0/(float)LOOP_HERTZ
#define LOOP_TIME_MS            LOOP_TIME*1000
#define LOOP_TIME_US            LOOP_TIME*1000000
#define DISPLAY_REFRESH_RATE    15                                  // Hz
#define DISPLAY_TIME            1.0/(float)DISPLAY_REFRESH_RATE
#define DISPLAY_TIME_MS         DISPLAY_TIME*1000
#define DISPLAY_TIME_US         DISPLAY_TIME*1000000
#define BAROMETER_REFRESH_RATE  2                                   // Hz
#define BAROMETER_TIME          1.0/(float)BAROMETER_REFRESH_RATE
#define BAROMETER_TIME_MS       BAROMETER_TIME*1000
#define BAROMETER_TIME_US       BAROMETER_TIME*1000000

enum JOYSTICK {
    UP,
    LEFT,
    DOWN,
    PRESS,
    RIGHT,
    NONE
};

enum LCD {
    ORIENTATION_PORTRAIT,
    ORIENTATION_LANDSCAPE
};

enum VARIO_STATE {
    HOME_VARIO,
    HOME_GPS,
    HOME_COMPASS,
    HOME_ALL,
    HOME_SETTINGS,
    VARIO,
    GPS,
    COMPASS,
    ALL,
    SETTINGS
};

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

JOYSTICK readButtons(int pin);
void beep(float smoothed_ascent_rate);
// Screen res 
//176*220
//driver: ILI9225