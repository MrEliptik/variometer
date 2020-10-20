#include <barometer.h>

SFE_BMP180 baro;

double baseline_pressure;
double baseline_altitude;

double last_pressure;
double last_altitude;

float sea_level_pressure = 1013.25;


int setupBarometer() {
    if (baro.begin()) {
        baseline_pressure = getAbsolutePressure();
        return 0;
    }
    else {
        return -1;
    }
}

float getTemperature() {
    char status;
    double T,P,p0,a;

    // First get a temperature measurment
    // If successful, the nb of ms to wait is returned
    // Otherwise, 0 is returned
    status = baro.startTemperature();
    if (status == 0) return(-1);

    // wait for the measurement to complete
    delay(status);

    // Retrieve the completed temparature measurement:
    // Note that the measurement is stored in the variable T
    // Use '&T' to provide the address of T to the function
    // Returns 1 if successful, 0 otherwise
    status = baro.getTemperature(T);
    if (status == 0) return(-1);

    return(T);
}

float getAbsolutePressure() {
    char status;
    double T,P,p0,a;

    T = getTemperature();
    if (T == -1) return(-1);

    // Start the pressure measurement:
    // The parameter is the oversampling setting, from 0 to 3 (higher res, longest wait)
    // If request if successful, the number of ms to wait is returned, 0 otherwise
    status = baro.startPressure(3);
    if (status == 0) return(-1);

    delay(status);

    // Retrieve the completed pressure measurement:
    // Note that the measurement is stored in the variable P
    // Use '&P' to provide the address of P
    // Note that the function requires the previous temperature measurement (T)
    // If temperature is stable, you can do one temperature measurement for a number of pressure measurements
    // Returns 1 if successful, 0 otherwise

    status = baro.getPressure(P, T);

    if (status == 0) return(-1);
    
    return(P);
}

float getAltitudeDelta() {
    double P;
    P = getAbsolutePressure();
    if (P == -1) return -1;

    return baro.altitude(P, baseline_pressure);
}

float getAltitude() {
    double P;
    P = getAbsolutePressure();
    if (P == -1) return -1;

    return baro.altitude(P, sea_level_pressure);
}