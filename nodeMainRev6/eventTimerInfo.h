//required information for eventTimer.h
//bytes for sensorRequest function calls
#define temperatureByte 1
#define humidityByte 2
#define moistureByte 3
#define waterlevelByte 4
#define lightSensorByte 5

//control pins
#define lightControlPin D6
#define fan1ControlPin D7   //NodeMCU pins should be set as D0,D1,D2 and so on... not just as 1,2,3...
#define pumpControlPin D5

//pump on time in ms
#define pumpSpinTime 3000
static int nextWateringTimeH = 0; //to store next watering time
