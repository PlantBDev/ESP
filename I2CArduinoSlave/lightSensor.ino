//Returns TRUE if amount of measured light is equal or exceeds the growing light

#define SENSORDEBUG 0

bool lightSensor(int lightSensorPin)
{
  int lightValue = 0;
  bool returnValue = false;
  lightValue = map(analogRead(lightSensorPin), 0, 1023, 0, 5000);
  /*#if SENSORDEBUG
    Serial.print("light value is: ");
    Serial.println(lightValue);
  #endif*/

  if(lightValue < 1000)
  {
    returnValue = true;
  }
  
  return returnValue;
}
