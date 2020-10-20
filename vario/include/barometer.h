#ifndef BAROMETER_H
#define BAROMETER_H

#include <Wire.h>
#include <SFE_BMP180.h>

extern SFE_BMP180 baro;

extern double baseline_pressure;
extern double baseline_altitude;

extern double last_pressure;
extern double last_altitude;

int setupBarometer();

float getTemperature();
float getPressure();
float getAltitude();

#endif