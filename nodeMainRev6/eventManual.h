
void eventManual(int light, int fan, int waterPump)
{
  #if EVENTTIMERDEBUG
    Serial.println("eventManual start");
  #endif
  if(light == 1)
  {
    digitalWrite(lightControlPin, true);
  }
  else if(light == 0)
  {
    digitalWrite(lightControlPin, false);
  }

  if(fan == 1)
  {
    digitalWrite(fan1ControlPin, true);
  }
  else if(fan == 0)
  {
    digitalWrite(fan1ControlPin, false);
  }
  if(waterPump == 1)
  {
    digitalWrite(pumpControlPin, true);
    delay(pumpSpinTime);
    digitalWrite(pumpControlPin, false);
  }
}