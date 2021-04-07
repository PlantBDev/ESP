#define MAIN

#define MQTT_PROT_DEBUG 1 //Misc. prints relating to MQTT data transfer
#define MQTTPUBDEBUG 1 // Prints about puplished messages
#define WIFIMANDEBUG 0 // Debug messages on the wifiManager header
#define IDENTITYDEBUG 1  //Debug prints relating to the "nodeIdentify" function

#define INTERVAL  10000 // How often to publish sensor data (ms)
#define NAMELOC 1 //Memory location on EEPROM where the nodes name is stored TODO reconsider if this is a good value for this

// Used ready made libraries
#include <EEPROM.h>
#include <ESP8266WiFi.h> // NodeMCu
#include <Ticker.h> // Used to keep timings consistent
#include <AsyncMqttClient.h> // MQTT client
#include <WiFiClient.h> 
#include <ESP8266WebServer.h> //Used in "wifiManager" header

ESP8266WebServer server(80);  //Port selection for the wifiManager

String formSsid;  //Wifimanager stores acquired credentials in these variables. TODO make these not global variables somehow?
String formPswd;

int nodeNameFound = 0; //Used to keep track of if the node has finished determining its ID. Also hold the new ID.  TODO move to main and look into making this not global if poosible
int nameCount[2] = {0,0};

// MQTT Broker connect info
#define MQTT_IP IPAddress(192, 168, 1, 236)
#define MQTT_PORT 1883

bool mqttconnected = 0; // TODO these don't have any good reason to be global variables other than general incompetence
bool wificonnected = 0;
bool messagepub = 0;

unsigned long previousMillis = 0;   // Stores last time data was published to make time between publishes semi consistent

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer; // Timer to determine how often to try to reconnect mqtt incase of disconnect
Ticker wifiReconnectTimer; // Timer to determine how often to try to reconnect wifi incase of disconnect

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;



void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2000);
  Serial.println();

  mqttSetup();

  wifiManager();  // Collect wifi name and password.
  connectToWifi();  // Attempt to connect to wifi. TODO fail safe for the situation where the wifi connection fails.

  nodeIdentify(); //Node figures out its identity
}


void loop() {
  if(mqttconnected == 1 && wificonnected == 1){

  /////////////////////////////  
  //// New sensor readings ////
  /////////////////////////////
  int tempTemp = random(20, 26);
  int tempSoilMoist = random(10, 100);
  int tempAirMoist = random(10, 100);
  int tempLight = random(0, 100);
  ///////////////////////////// TODO implement sensor functions above.
    
  /////////////////////////////////  
  //// Sensor readings publish ////
  /////////////////////////////////
  }

  if(messagepub == 1){
    delay(5000);
    //ESP.deepSleep(20e6); // Sleep to reduce energy consumption, 20e6 microseconds is 20 seconds
  }
}
