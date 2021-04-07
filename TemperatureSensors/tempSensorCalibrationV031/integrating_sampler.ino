long integrating_sampler(int pin)
{
  int numberOfSamples = 500; // should be high enoug that atleast one plc (50Hz) is fully captured
  int delayPerSample = 1; //delay between two samples

  long voltage = 0;
  int analogReading[numberOfSamples];

  for(int i = 0; i < numberOfSamples; i++)
  {
    analogReading[i] = map(analogRead(pin),0,1023,0,5000);
    delay(delayPerSample);
  }

  for(int i = 0; i < numberOfSamples; i++)
  {
    voltage += analogReading[i];
  }
  voltage = voltage/numberOfSamples;
  
  return voltage;
}
