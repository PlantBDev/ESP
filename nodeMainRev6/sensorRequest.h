//sensorRequest() returns sensor value from connected I2C slave device 
//required sensor can be chosen by giving the function byte from 1 to 5 as a parametre
/*Sensor number reference:
                          temperature = 1
                          humidity    = 2
                          moisture    = 3
                          waterlevel  = 4
                          light       = 5
*/
#define SENSORATTEMPTS 20  //How many times flawed measurements are retried before giving up and returning error value(-888888888)

#include <Wire.h>;

#ifndef MAIN
#define SENSORDEBUG 1 //Toggles debug prints related to this function
#endif

int sensorRequest(byte sensorNum){
 int32_t sensorData; 
 byte a,b,c,d;
 bool sensorValueVerified = 0;
 int attemptCount = 0;
 Wire.begin(D1,D2);  // join i2c bus with SDA=D1 and SCL=D2 of NodeMCU

 while(sensorValueVerified == 0 && attemptCount <= SENSORATTEMPTS ){
  delay(3000);
  Wire.beginTransmission(54); // begin transmission with device address 54
  Wire.write(sensorNum);  // Tell slave device which sensors data has been requested
  Wire.endTransmission();  // stop transmitting
  #if SENSORDEBUG
  Serial.print("\nRequested datatype: ");
  Serial.println(sensorNum);
  #endif
 
  Wire.requestFrom(54,4);   //Sending request for sensor data which is going to be a 32bit integer making it too big to be received
  while(Wire.available()){  //in just 1 piece.(I2C can deliver only one byte at a time and 32bits = 4bytes meaning we need 4 packets)
    a = Wire.read();
    b = Wire.read();
    c = Wire.read();
    d = Wire.read();
  }
 
  // Combine the received bytes to the sensorData. Example of how this works: a=01010101, b=00001111, c=11110000 and sensorData=32zeroes
  sensorData = a;                                 //=>  sensorData=00000000 00000000 00000000 01010101
  sensorData = (sensorData << 8) | b;             //=>  sensorData=00000000 00000000 01010101 00001111
  sensorData = (sensorData << 8) | c;             //=>  sensordata=00000000 01010101 00001111 11110000
  sensorData = (sensorData << 8) | d;             //etc.

  if(sensorNum == 1 && sensorData <= 34 && sensorData >= 0){
    sensorValueVerified = 1;
  }
  else if(sensorNum == 2 && sensorData <= 1000 && sensorData >= -10){
    sensorValueVerified = 1;
  }
  else if(sensorNum == 3 && sensorData <= 100 && sensorData >= 0){
    sensorValueVerified = 1;
  }
  else if(sensorNum == 4 && sensorData <= 100 && sensorData >= 0){
    sensorValueVerified = 1;
  }
  else if(sensorNum == 5 && sensorData <= 1 && sensorData >= 0){
    sensorValueVerified = 1;
  }
  else if(sensorNum > 5 || sensorNum < 1){
    attemptCount = SENSORATTEMPTS;
  }
  else{
   attemptCount++;
  }
 }
  
 if(sensorData == 888888888 || sensorData <= -8496 || sensorValueVerified == 0){
  #if SENSORDEBUG
  Serial.println("There was a issue with I2C");
  #endif
  switch(sensorNum){
    case 1:
     tempSensorBroke = 1;
     #if SENSORDEBUG
     Serial.println("Problem with temperature sensor");
     #endif
    break;
    case 2:
     humiditySensorBroke = 1;
     #if SENSORDEBUG
     Serial.println("Problem with humidity sensor");
     #endif
    break;
    case 3:
     moistureSensorBroke = 1;
     #if SENSORDEBUG
     Serial.println("Problem with moisture sensor");
     #endif
    break;
    case 4:
     waterLevelSensorBroke = 1;
     #if SENSORDEBUG
     Serial.println("Problem with waterlevel sensor");
     #endif
    break;
    case 5:
     lightSensorBroke = 1;
     #if SENSORDEBUG
     Serial.println("Problem with light sensor");
     #endif
    break;
    default:
     #if SENSORDEBUG
     Serial.println("Error in sensor selection");
     #endif
    break;
  }
  return 888888888;  //TODO think of a value to return when mistakes were made
 }
 else{
  #if SENSORDEBUG
  Serial.print("Received sensor data: ");
  Serial.println(sensorData);
  #endif
  return sensorData; //returns sensorData as a 32bit integer
 }
}
