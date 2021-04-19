//required information for eventTimer.h
//bytes for sensorRequest function calls
#define temperatureByte 1
#define humidityByte 2
#define moistureByte 3
#define waterlevelByte 4
#define lightSensorByte 5

//control pins
#define lightControlPin 10
#define fan1ControlPin 10
#define pumpControlPin 10

//pump on time in ms
#define pumpSpinTime 500
static int nextWateringTimeH = 0; //to store next watering time
