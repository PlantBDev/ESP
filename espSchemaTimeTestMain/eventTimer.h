#include "NTPclock.h" //to get time from NTP server
#include "eventTimerInfo.h" //ALL control pins are defined here!!
#include "sensorRequestDummy.h" //placeholder for sensorRequest function
//TODO improve the usefulness of input variables
void eventTimer(int lightOnTime, int tempLightOn, int tempLightOff, int humLightOn, int humLightOff, int moistureLightOn, int moistureLightOff)
{
  //------------------------------------------------------LIGHTS
  int lightTurnOffTime = 0; //keeps track of when to turn the lights off
  int * currentTime; //to store current time address
  bool lightStatus = false; //variable for tracking current light status
  bool lightLevel = sensorRequest(lightSensorByte); //store current light level
  currentTime = NTPclock(); //get current time

  //calculate light turnoff time
    //TODO handling of situations where turnoff time is on the next day
  lightTurnOffTime = dayStartTimeH + lightOnTime;
  
  //control lights
  if(currentTime[0] < lightTurnOffTime && !lightLevel) //check if lights should be on from clock and if lights are needed at all from ambient light sensor
  {
    lightStatus = true;
    digitalWrite(lightControlPin, true);
  }
  else
  {
    lightStatus = false;
    digitalWrite(lightControlPin, false);
  }
  //------------------------------------------------------SOIL MOISTURE
  int currentSoilMoisture = sensorRequest(moistureByte); //get current moisture
  if(currentSoilMoisture < moistureLightOn && *currentTime > nextWateringTimeH)
  {
    digitalWrite(pumpControlPin, true);
    delay(pumpSpinTime);
    digitalWrite(pumpControlPin, false);
  }
  

  //------------------------------------------------------HUMIDITY
  //Both humidity and temperature is currently handled by the same fan. Temperature condition overwrites humidity control!
  int currentHumidity = sensorRequest(humidityByte);
  if(currentHumidity > humLightOn)
  {
   digitalWrite(fan1ControlPin, true);
  }
  else
  {
    digitalWrite(fan1ControlPin, false);
  }
    //------------------------------------------------------TEMPERATURE
  //------------------------------------------------------TODO handling ambient temperature being greater than desired temperature
  int currentTemperature = 0;
  currentTemperature = sensorRequest(temperatureByte); //get temperature from arduino
  if(currentTemperature > tempLightOn && lightStatus) //check if greenhouse should be cooled
  {
    digitalWrite(fan1ControlPin, true);
  }
  else
  {
    digitalWrite(fan1ControlPin, false);
  }
}
