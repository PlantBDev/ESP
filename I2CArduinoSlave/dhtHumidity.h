#include <DHT.h> //Kirjaston sisällytys
//#define measurePin 6 //Pin that measures the dht
#define DHTTYPE DHT11 //Sensors' type

int32_t dht(int measurePin){ //The name of the function
 //Serial.println("Help");
  int hum = 69420;
  DHT dht(measurePin, DHTTYPE);
  dht.begin();
  hum = dht.readHumidity();        //Read humidity
 
 if (isnan(hum)) {
   hum = 69420;
   return hum;
  }
return hum;
}
