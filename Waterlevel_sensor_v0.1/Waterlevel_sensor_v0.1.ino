#ifndef MAIN
#define DEBUG 1
// This constant won't change
const int analogInPin = A1; // Analog input pin that the sensor is attached to
int waterAnal();
int sensorValue; // value read from the sensor

void setup() {
// initialize serial communications at 9600 bps:
Serial.begin(9600);
}
void loop (){
  waterAnal();
}
#endif
int waterAnal() {
// read the analog in value:
sensorValue = analogRead(analogInPin);
// map it to the range of the analog out:
sensorValue = map(sensorValue, 0, 1023, 0, 100);
#if DEBUG
// print the results to the Serial Monitor:
Serial.print("water level= ");
Serial.print(sensorValue);
Serial.println("%");
#endif
return sensorValue;
}
