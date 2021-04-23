//This is a test environment for the "sensorRequest" function

#include "sensorRequest.h"

void setup(){
 Serial.begin(115200);
}
 
void loop(){
 for(int i=0;i<=5;i++){  //Run sensorRequest with cycling parametres
  sensorRequest(random(0,6));
  delay(4000);
 }
}
