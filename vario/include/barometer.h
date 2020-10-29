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

double getTemperature();
double getAbsolutePressure();
double getAbsolutePressure(double temp);
double getAltitudeDelta();
double getAltitudeDelta(double pressure);
double getAltitude();
double getAltitude(double pressure);
double getAscentRate(double last_reading_time);
double getAscentRate(double altitude, unsigned long last_reading_time);

#endif