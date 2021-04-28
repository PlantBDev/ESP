


//-----------------------------This is a test main for voltsToTemp function
#include"voltsToTemp.h"
void setup() {
  Serial.begin(9600);
  Serial.print("hello");
}

void loop() {
  //int x = voltsToTemp(analogRead(0));
  Serial.println(analogRead(0));
  delay(3000);
}
