//Basic idea behind the code is that master device sends request for one kind of sensor data 
//and Arduino responds with that data when master device goes to request mode.

//To get the desired output master device needs to first send byte with value from 1 to 5 
//to the slave device to determine which sensors data it wants, and then send request for the data.
/*Sensor number reference:
                          temperature = 1
                          humidity    = 2
                          moisture    = 3
                          waterlevel  = 4
                          light       = 5
*/

#define MAIN
#include"voltsToTemp.h"
#include"dhtHumidity.h"

#define SENSORDEBUG 1 // Can be used to toggle debug prints(should be toggled off)

/* Pins for the sensors */
#define TEMPPIN A0
#define MOISTPIN A1
#define WATERLEVELPIN A3
#define HUMIDITYPIN A2
#define LIGHTPIN A4

#include <Wire.h>;
int32_t humidity = 0;

int32_t sensorData = 888888888;

byte sensorNum = 0;
#if SENSORDEBUG
bool sensorWasRead = 0;
#endif

void setup(){
 Wire.begin(54);
 Wire.onReceive(receiveEvent); //"receiveEvent" function gets called when I2C message is received
 Wire.onRequest(requestEvent); //"requestEvent" function gets called when I2C request is received
 Serial.begin(115200); //For debug. Serial functions should not be used inside event functions relating to I2C it causes hard to find issues!!
 #if SENSORDEBUG

 delay(100);

 Serial.println(" I2C Time ");
 #endif
}
 
void loop(){

 delay(3000);
 humidity = dht(HUMIDITYPIN);
 // Serial.print("k"); //Highly valuable debug tool

 #if SENSORDEBUG
 if(sensorWasRead){
  Serial.print("Read from: ");
  Serial.println(sensorNum);
  Serial.print("Sending: ");
  Serial.println(sensorData);
  Serial.print("\n");
  sensorWasRead = 0;
 }
 #endif
}




void receiveEvent(int len) {
  sensorNum = Wire.read(); //Receive the sensorNum from the master device
  switch (sensorNum) {    //Make measurements with sensor corresponding to sensorNum
  case 1:  // Temperature
    sensorData = voltsToTemp(analogRead(TEMPPIN));
    //sensorData = 1000000; //For debug
    break;
  case 2:  // Humidity
    sensorData = humidity;

    //sensorData = -84970000;  //For debug
    break;
  case 3:  // Moisture
    sensorData = moistAnal(MOISTPIN);
    //sensorData = 313;  //For debug
    break;
  case 4:  // WaterLevel
    sensorData = waterAnal(WATERLEVELPIN);
    //sensorData = 1234;  //For debug
    break;
  case 5:  // Light
    sensorData = lightSensor(LIGHTPIN);
    //sensorData = 54321;  //For debug
    break;
  default:
    sensorData = 888888888;
    break;
 }
 #if SENSORDEBUG
 sensorWasRead = 1;
 #endif
}




void requestEvent(){  //Chop sensorData to byte sized pieces and send them to the master device
 byte myArray[4];  
 myArray[0] = (sensorData >> 24) & 0xFF;
 myArray[1] = (sensorData >> 16) & 0xFF;
 myArray[2] = (sensorData >> 8) & 0xFF;
 myArray[3] = sensorData & 0xFF;
 Wire.write(myArray, 4);
}
