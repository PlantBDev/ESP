#include "NTPclock.h"
//TODO improve the usefulness of input variables
void eventTimer(int lightOnTime, int tempLightOn, int tempLightOff, int humLightOn, int humLightOff, int moistureLightOn, int moistureLightOff)
{
  int lightTurnOffTime = 0; //keeps track of when to turn the lights off
  int * currentTime; //to store current time address
  currentTime = NTPclock(); //get current time

  //calculate light turnoff time
    //TODO handling of situations where turnoff time is on the next day
  lightTurnOffTime = dayStartTimeH + lightOnTime;
  //control lights
  if(currentTime[0] < lightTurnOffTime)
  {
    //-----------------------------------------------TODO light control pin output TRUE here!--------------------------------------------------------------
  }
  else
  {
    //-----------------------------------------------TODO light control pin output FALSE here!-------------------------------------------------------------- 
  }
}
