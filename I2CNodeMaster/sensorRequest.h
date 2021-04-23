//sensorRequest() returns sensor value from connected I2C slave device 
//required sensor can be chosen by giving the function byte from 1 to 5 as a parametre
/*Sensor number reference:
                          temperature = 1
                          humidity    = 2
                          moisture    = 3
                          waterlevel  = 4
                          light       = 5
*/

#include <Wire.h>;

#ifndef MAIN
#define SENSORDEBUG 1 //Toggles debug prints related to this function
#endif

int sensorRequest(byte sensorNum){
 int32_t sensorData; 
 byte a,b,c,d;

 Wire.begin(D1,D2);  // join i2c bus with SDA=D1 and SCL=D2 of NodeMCU
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
 if(sensorData == 888888888 || sensorData == -8496){
  #if SENSORDEBUG
  Serial.println("There was a issue with I2C");
  #endif
  return sensorData;  //TODO think of a value to return when mistakes were made
 }
 else{
  #if SENSORDEBUG
  Serial.print("Received sensor data: ");
  Serial.println(sensorData);
  #endif
  return sensorData; //returns sensorData as a 32bit integer
 }
}
