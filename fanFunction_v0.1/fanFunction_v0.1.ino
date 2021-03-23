#ifndef MAIN
#define DEBUG 1 //This is for debugging purposes
void fan(); //The name of the function
int relay_pin = 9; //Pin that controls the fan, if it's high fan is ON and if it's low, the fan is OFF
int fanTime = 2000; //Wanted time the fan is on (will be added to MAIN later)
pinMode(relay_pin, OUTPUT);
digitalWrite(relay_pin, LOW); //Fan is off by default

void setup() { 
  Serial.begin(9600); // initialize serial communications at 9600 bps:
}

void loop() { 
  fan();
}
#endif
void fan(){
  digitalWrite(relay_pin, HIGH);
  delay(fanTime); //The wanted time that the fan is on
  digitalWrite(relay_pin, LOW);
#if DEBUG
// Print the state of the fan to the Serial Monitor:
  if (relay_pin == HIGH){
    Serial.println("The fan is ON");
    delay (10);
}
  else {
    Serial.println("Fan is OFF ");
    delay (10);
}
#endif
return;
}
