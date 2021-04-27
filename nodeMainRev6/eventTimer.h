 #include "NTPclock.h" //to get time from NTP server
#include "eventTimerInfo.h" //ALL control pins are defined here!!

//TODO improve the usefulness of input variables
void eventTimer(int lightOnTime, int tempLightOn, int tempLightOff, int humLightOn, int humLightOff, int moistureLightOn)
{
  //------------------------------------------------------LIGHTS
  int lightTurnOffTime = 0; //keeps track of when to turn the lights off
  int * currentTime; //to store current time address
  bool currentStatus[3] = {false, false, false}; //keeps track if outputs change
  bool lightLevel = sensorRequest(lightSensorByte); //store current light level
  currentTime = NTPclock(); //get current time

  //calculate light turnoff time
    //TODO handling of situations where turnoff time is on the next day
  lightTurnOffTime = dayStartTimeH + lightOnTime;

  //control lights
  #if EVENTTIMERDEBUG
  Serial.println("Light control");
  #endif
  if(currentTime[0] < lightTurnOffTime && !lightLevel) //check if lights should be on from clock and if lights are needed at all from ambient light sensor
  {
    currentStatus[0] = true;
    digitalWrite(lightControlPin, true);
  }
  else
  {
    currentStatus[0] = false;
    digitalWrite(lightControlPin, false);
  }


  //------------------------------------------------------SOIL MOISTURE
  #if EVENTTIMERDEBUG
  Serial.println("Water control");
  #endif
  int currentSoilMoisture = sensorRequest(moistureByte); //get current moisture
  if(currentSoilMoisture < moistureLightOn && *currentTime > nextWateringTimeH)
  {
    //TODO how to properly report pump usage
    nextWateringTimeH = *currentTime + 1; //set the next watering time to next hour
    digitalWrite(pumpControlPin, true);
    currentStatus[2] = true;                //the use of currentStatus here is useless, as it is never reported before it is changed back again...
    delay(pumpSpinTime);                    //TODO change ho the pump is controlled. without delay but with clock.(if that makes any sense to do so.)
    digitalWrite(pumpControlPin, false);
    currentStatus[2] = false;
  }

  //------------------------------------------------------HUMIDITY
  //Both humidity and temperature is currently handled by the same fan. Temperature condition overwrites humidity control!
  #if EVENTTIMERDEBUG
  Serial.println("Humidity control");
  #endif
  int currentHumidity = sensorRequest(humidityByte);
  if(currentHumidity > humLightOn)
  {
   digitalWrite(fan1ControlPin, true);
   currentStatus[1] = true;
  }
  else
  {
    digitalWrite(fan1ControlPin, false);
    currentStatus[1] = false;
  }

    //------------------------------------------------------TEMPERATURE
  //------------------------------------------------------TODO handling ambient temperature being greater than desired temperature
  int currentTemperature = 0;
  #if EVENTTIMERDEBUG
  Serial.println("Temp control");
  #endif
  currentTemperature = sensorRequest(temperatureByte); //get temperature from arduino
  if(currentTemperature > tempLightOn && currentStatus[0]) //check if greenhouse should be cooled //currentStatus[0] is light status
  {
    digitalWrite(fan1ControlPin, true);
    currentStatus[1] = true;
  }
  else
  {
    digitalWrite(fan1ControlPin, false);
    currentStatus[1] = false;
  }
  //determine if actuator states have changed since last execution
  if(currentStatus[0] != previousState[0] && currentStatus[1] != previousState[1] && currentStatus[2] != previousState[2])
  {
    mqttClient.publish("node/actuator/id", 2, false, "node id here"); //TODO get node id!-----------------------------------------------------
    if(currentStatus[0])//report light status
    {
      mqttClient.publish("node/actuator/light", 2, false, "true");
    }
    else
    {
      mqttClient.publish("node/actuator/light", 2, false, "false");
    }
    if(currentStatus[1])//report fan status
    {
      mqttClient.publish("node/actuator/fan1", 2, false, "true");
    }
    else
    {
      mqttClient.publish("node/actuator/fan1", 2, false, "false");
    }
    if(currentStatus[1])//report pump status
    {
      mqttClient.publish("node/actuator/waterPump", 2, false, "true");
    }
    else
    {
      mqttClient.publish("node/actuator/waterPump", 2, false, "false");
    }
  }
  //store current status as previous for next execution
  previousState[0] = currentStatus[0];
  previousState[1] = currentStatus[1];
  previousState[2] = currentStatus[2];

}
