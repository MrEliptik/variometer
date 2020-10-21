#include <Arduino.h>
#include <barometer.h>

// Need to use GPIO number
#define GPS_PPS_PIN 0
#define LED_PIN 3
#define BUZZER_PIN 14
#define SLCK_PIN 5
#define MISO_PIN 6
#define MOSI_PIN 7
#define CS_PIN 8

#define JOYSTICK_PIN A0

int readButtons(int pin);
void beep(float smoothed_ascent_rate);
// Screen res 
//176*220
//driver: ILI9225