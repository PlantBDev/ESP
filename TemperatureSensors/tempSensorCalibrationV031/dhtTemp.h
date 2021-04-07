int dhtTemp(){
  int check = DHT11.read(DHT11Pin);
  int temp = DHT11.temperature;
  return temp;
}
