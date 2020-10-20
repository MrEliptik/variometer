#include <main.h>

double lastTime;

void setup() {
  Serial.begin(115200);
  Serial.print("VARIOMETER v0.1 booting..");
  
  setupBarometer();

}

void loop() {
  //lastTime = millis();
  Serial.print("Atltitude: ");
  Serial.println(getAltitude());

  Serial.print("Temperature: ");
  Serial.println(getTemperature());
  //Serial.print("Time to get measure: ");
  //Serial.println(millis() - lastTime);
  delay(1000);
}