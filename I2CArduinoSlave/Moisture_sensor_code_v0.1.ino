/*#ifndef MAIN
#define DEBUG 1
//Setup
int sensorPin = A0;
int outputValue ;
int moistAnal();
void setup() 
{
   Serial.begin(9600);
   #if DEBUG
   Serial.println("Reading Data From the Sensor ...");
   delay(2000);
   #endif
}
void loop (){
  moistAnal();
}
#endif*/



int32_t moistAnal (int sensorPin){
   int32_t outputValue= analogRead(sensorPin);

   outputValue = map(outputValue,0,1024,0,100);

  //Changing the moisture level to percents
  /*#if SENSORDEBUG
   Serial.print("Moisture Value : ");
   Serial.print(outputValue);
   Serial.println("%");
   delay(1000);
  #endif*/
   return outputValue;
  }
