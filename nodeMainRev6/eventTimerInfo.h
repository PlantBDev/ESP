//required information for eventTimer.h
//bytes for sensorRequest function calls
#define temperatureByte 1
#define humidityByte 2
#define moistureByte 3
#define waterlevelByte 4
#define lightSensorByte 5

//control pins
#define lightControlPin 10  //TODO Change these before test 
#define fan1ControlPin 10   //To remember nodemcu pins should be set as D0,D1,D2 and so on... not just as 1,2,3...
#define pumpControlPin 10

//pump on time in ms
#define pumpSpinTime 500
static int nextWateringTimeH = 0; //to store next watering time

//arrays to keep track of the state of the actuators
bool previousState[3] = {false, false, false}; //order is light, fan, pump
