#define DHT11Pin 4
#include<dht11.h>
dht11 DHT11;
#include"dhtTemp.h"
void setup() {
  Serial.begin(9600);
  Serial.println("setup");

}

void loop() {
  long sample0;
  long sample2;
  long sample3;
  int actualTemperature = 0;

  sample0 = integrating_sampler(0);
  //sample2 = integrating_sampler(2);
  //sample3 = integrating_sampler(3);
  actualTemperature = dhtTemp();

  Serial.print(sample0);
  Serial.print(" ");
  /*Serial.print(sample2);
  Serial.print(" ");
  Serial.print(sample3);
  Serial.print(" ");*/
  Serial.println(actualTemperature);
  delay(30000);
}
