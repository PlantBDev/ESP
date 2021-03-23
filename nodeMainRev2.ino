#define MAIN
#define INTERVAL  10000 // How often to publish sensor data (ms)
#define MQTT_PROT_DEBUG 1 //Misc. prints relating to MQTT data transfer
#define MQTTPUBDEBUG 1 // Prints about puplished messages

// Used ready made libraries
#include <ESP8266WiFi.h> // NodeMCu
#include <Ticker.h> // Used to keep timings consistent
#include <AsyncMqttClient.h> // MQTT client

// Wifi credentials, TODO implement "get_wifi_credentials.h"
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

// MQTT Broker connect info
#define MQTT_IP IPAddress(192, 168, 1, 236)
#define MQTT_PORT 1883

// MQTT Topics where the data will be submited to, TODO rethink publish structure, maybe good enuf?
#define MQTT_PUB_TEMP "node/temp"
#define MQTT_PUB_HUM "esp/hum"

bool mqttconnected = 0;
bool wificonnected = 0;
bool messagepub = 0;

unsigned long previousMillis = 0;   // Stores last time temperature was published

AsyncMqttClient mqttObj;
Ticker mqttReconnectTimer; // Timer to determine how often to try to reconnect mqtt incase of disconnect
Ticker wifiReconnectTimer; // Timer to determine how often to try to reconnect wifi incase of disconnect

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(2000);
  Serial.println();

  mqttconnected = 0;
  wificonnected = 0;
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  
  mqttObj.onConnect(onMqttConnect);
  mqttObj.onDisconnect(onMqttDisconnect);
  mqttObj.onSubscribe(onMqttSubscribe);
  mqttObj.onPublish(onMqttPublish);
  mqttObj.setServer(MQTT_IP, MQTT_PORT);
  
  connectToWifi();

  // TODO subscribing to system/nodecount and getting node number from the topic 
  int nodeCount = 2;

  String nodeName = String(node);
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
    
    // Publish an MQTT message on topic nodeN/sensors/temp
    uint16_t packetIdPub1 = mqttObj.publish(MQTT_PUB_TEMP, 1, true, String(temp).c_str());
    #if MQTTPUBDEBUG       
    Serial.printf("Publishing on topic %s at QoS 1, packetId: %i ", "nodeN/sensors/temp", packetIdPub1);
    Serial.printf("Message: %.2f \n", temp);
    #endif
 
    // Publish an MQTT message on topic Node/sensors/hum
    uint16_t packetIdPub2 = mqttObj.publish(MQTT_PUB_HUM, 1, true, String(hum).c_str());
    #if MQTTPUBDEBUG                            
    Serial.printf("Publishing on topic %s at QoS 1, packetId %i: ", "nodeN/sensors/hum", packetIdPub2);
    Serial.printf("Message: %.2f \n", hum);
    #endif
    messagepub = 1;
  }

  if(messagepub == 1){
    delay(5000);
    ESP.deepSleep(20e6); // Sleep to reduce energy consumption, 20e6 microseconds is 20 seconds
  }
}




void connectToWifi() {
  #if MQTT_PROT_DEBUG
  Serial.println("Connecting to Wi-Fi...");
  #endif
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  #if MQTT_PROT_DEBUG
  Serial.println("Connected to Wi-Fi.");
  #endif
  wificonnected = 1;
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  #if MQTT_PROT_DEBUG
  Serial.println("Disconnected from Wi-Fi.");
  #endif
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}

void connectToMqtt() {
  #if MQTT_PROT_DEBUG
  Serial.println("Connecting to MQTT...");
  #endif
  mqttObj.connect();
}

void onMqttConnect(bool sessionPresent) {
  #if MQTT_PROT_DEBUG
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  #endif
  mqttconnected = 1;
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  #if MQTT_PROT_DEBUG
  Serial.println("Disconnected from MQTT.");
  #endif
  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  #if MQTT_PROT_DEBUG
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
  #endif
}

void onMqttPublish(uint16_t packetId) {
  #if MQTT_PROT_DEBUG
  Serial.print("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  #endif
}
