#include <main.h>

double lastTime;

void setup() {
  Serial.begin(115200);
  Serial.print("VARIOMETER v0.1 booting..");
  
  setupBarometer();

}

void loop() {
  //lastTime = millis();

  Serial.println("");
  Serial.println("##############");

  Serial.print("Temperature: ");
  Serial.println(getTemperature());

  Serial.print("Pressure: ");
  Serial.println(getPressure());

  Serial.print("Altitude: ");
  Serial.println(getAltitude());

  Serial.print("Altitude delta: ");
  Serial.println(getAltitudeDelta());


  //Serial.print("Time to get measure: ");
  //Serial.println(millis() - lastTime);
  delay(1000);
}