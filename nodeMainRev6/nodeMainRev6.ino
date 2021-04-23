#define MAIN

#define MQTT_PROT_DEBUG 1 //Misc. prints relating to MQTT data transfer
#define MQTTPUBDEBUG 1 // Prints about puplished messages
#define WIFIMANDEBUG 0 // Debug messages on the wifiManager header
#define IDENTITYDEBUG 1  //Debug prints relating to the "nodeIdentify" function
#define SENSORDEBUG 1 //Toggles debug prints related to this function

#define MQTTDEBUG 1 //

#define DANGERDEBUG 1
#define MQTT_MSG_DEBUG 1


#define INTERVAL  10000 // How often to publish sensor data (ms) TODO unused at the moment
#define NAMELOC 1 //Memory location on EEPROM where the nodes name is stored TODO reconsider if this is a good value for this

// Used ready made libraries
#include <EEPROM.h>
#include <ESP8266WiFi.h> // NodeMCu
#include <Ticker.h> // Used to keep timings consistent
#include <AsyncMqttClient.h> // MQTT client
#include <WiFiClient.h> 
#include <ESP8266WebServer.h> //Used in "wifiManager" header

ESP8266WebServer server(80);  //Port selection for the wifiManager
AsyncMqttClient mqttClient;

String formSsid;  //Wifimanager stores acquired credentials in these variables. TODO make these not global variables somehow?
String formPswd;


//String formSsid = "";  //Wifimanager stores acquired credentials in these variables. TODO make these not global variables somehow?
//String formPswd = "";


String formSsid = "lassila";  //Wifimanager stores acquired credentials in these variables. TODO make these not global variables somehow?
String formPswd = "56310460";

//Checks for sensors with issues
bool tempSensorBroke = 0;
bool humiditySensorBroke = 0;
bool moistureSensorBroke = 0;
bool waterLevelSensorBroke = 0;
bool lightSensorBroke = 0;


int nodeNameFound = 0; //Used to keep track of if the node has finished determining its ID. Also hold the new ID.  TODO move to main and look into making this not global if poosible
int nameCount[2] = {0,0};

int schematicArray[6]; //Array that stores schematic data in mqtt events and makes it available in main
int schematicProgress = 0; //Stores schematic reading progress in mqttHead and marks schematicArray as safe to read when reading is finished

//#include"wifiCredDebug.h"
#include"sensorRequest.h"
#include"mqttPub.h"
#include"topicVerification.h"


// MQTT Broker connect info
#define MQTT_IP IPAddress(192, 168, 1, 236)
#define MQTT_PORT 1883

bool wifiManConf = 0;
bool mqttconnected = 0; // TODO these don't have any good reason to be global variables other than general incompetence
bool wificonnected = 0;
bool schematicAcquired = 0;
bool messagepub = 0;

unsigned long previousMillis = 0;   // Stores last time data was published to make time between publishes semi consistent

Ticker mqttReconnectTimer; // Timer to determine how often to try to reconnect mqtt incase of disconnect
Ticker wifiReconnectTimer; // Timer to determine how often to try to reconnect wifi incase of disconnect

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

int32_t temperature = 888888888; 
int32_t humidity = 888888888;
int32_t soilMoisture = 888888888;
int32_t waterLevel = 888888888;
int32_t light = 888888888;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2000);
  Serial.println();

  //commented out for less complex testing. Network information is thus hardcoded in this file!!!
  //wifiManager();  // Collect wifi name and password.

  mqttSetup();

  while(wificonnected != 1){
  connectToWifi();  // Attempt to connect to wifi. TODO fail safe for the situation where the wifi connection fails.
  delay(5000);
  }
  while(mqttconnected != 1){
  delay(1000);
  }
  
  nodeIdentify(); //Node figures out its identity
  
  mqttClient.subscribe("server/id", 2);  //Subscribe to the server/id topic which serves schematics to the nodes.
  //while(schematicAcquired == 0){         //Wait for the schematic to be served
    delay(100);                   //TODO Incase of a issue here there should be a safety feature, maybe node resets after too many loops
  //}
  mqttClient.subscribe("node/id", 2);  //Subscribe to the node/id topic where the nodes id will be published when it's turn to publish sensor readings
  mqttClient.subscribe("node/danger/id", 2); //Subscribe to the node/id topic where the nodes id will be published when it's turn to publish danger statuses
}


void loop() {

  if(mqttconnected == 1 && wificonnected == 1){
    //   New sensor readings   //
    temperature = sensorRequest(1); 
    humidity = sensorRequest(2);
    soilMoisture = sensorRequest(3);
    waterLevel = sensorRequest(4);
    light = sensorRequest(5);
  
    ///////////////////////////////////////  
    //// Managing the functional parts ////  //Add schematic subscribe and wait loop before any actuator changes
    ///////////////////////////////////////
    //Below is a test for mqttHead schematic receiving
    if(schematicProgress == 8)
    {
      Serial.println("It's now safe to use schematicArray");
      for(int i = 0; i <= 6; i++)
      {
        Serial.print(schematicArray[i]);
        Serial.print(" ");
      }
      Serial.println(" This is the content of schematicArray");
      schematicProgress = 0; //this has to be done after the data is read and transferred elsewhere
    }
  }


  delay(5000);
}
