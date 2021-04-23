#ifndef MAIN
#define MQTTDEBUG 0
#define MAINIAEIENAANOLEKAIKKIONMENETETTY

#include <ESP8266WiFi.h>
#include <Ticker.h>
#include <AsyncMqttClient.h>

#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define MQTT_HOST IPAddress(192, 168, 1, 246)
#define MQTT_PORT 1883

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onUnsubscribe(onMqttUnsubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);

  connectToWifi();
}

void loop() {
  nodeIdentify();
  #ifdef MAINIAEIENAANOLEKAIKKIONMENETETTY
  delay(10000);
  #endif
}
#endif


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void nodeIdentify(){
EEPROM.begin(10); 
#ifndef IDENTITYDEBUG
delay(2000);
#endif

/*EEPROM.write(NAMELOC+1, 128);  //Can be used to set custom ID to EEPROM for debug purposes
  EEPROM.commit();
  EEPROM.write(NAMELOC+2, 64);
  EEPROM.commit();
  EEPROM.write(NAMELOC+3, 32);
  EEPROM.commit();*/

int emptyTopicCount = 3;  //How many empty topics are tolerated before "giving up" and node renaming itself
char  topicString[15]="node/Count000"; //the max. string length is 14 characters + Null as string-terminator 
uint8_t   InputInt[10]={48,49,50,51,52,53,54,55,56,57}; //48 ist the character '0' in ASCII, 49 is the '1' and so on
int l = strlen(topicString);
nodeNameFound = 0;
nameCount[0] = 0;
nameCount[1] = 0;

for(int i = 0; i < 10; i++){  //Nested for loops for incrementing topic number, this limits max number of nodes to 999.
  topicString[l-3]=InputInt[i];
  for(int j = 0; j < 10; j++){
    topicString[l-2] = InputInt[j];
    for(int k = 0; k < 10; k++){
      if(nodeNameFound != 0){
        mqttClient.unsubscribe(topicString); //Unsubscribe from node/CountNNN topic
        Serial.println("Node identify success!");
        Serial.print("Nodes ID:");
        Serial.println(nodeNameFound);
        return;
        }
      else{
        topicString[l-1]=InputInt[k];
        mqttClient.subscribe(topicString, 2);  //Subscribe to the different "node/countNNN" mqtt topics which contain a list of node identifiers
        #if IDENTITYDEBUG
        Serial.print("Subscribing to topic: \"");
        Serial.print(topicString);
        Serial.println("\"");
        #endif
        delay(3000);  //Wait for 5 seconds for retained message
        mqttClient.unsubscribe(topicString); //Unsubscribe from previous node/CountNNN topic
        nameCount[1] = nameCount[1]+1;  //Name count arrays values are incremented when MQTT message is received and when the MQTT topic is unsubscribed
                                        //Which means difference between nameCount[1] and nameCount[0] can be used to keep track of topics that didn't
        #if IDENTITYDEBUG               //have retained messages.
        Serial.print("Unsubscribing from topic: \"");
        Serial.print(topicString);
        Serial.println("\"");
        #endif
        
        if(nameCount[1] - nameCount[0] > emptyTopicCount){  //If there have been more than "emptyTopicCount" "empty" name/CountNNN topics.  TODO determine the acceptable amount of empty topics
          for(int i = 0; i < 4; i++){       //Removing the old ID information from the EEPROM
            EEPROM.write(NAMELOC+i, 0);
            }
          int newNamePart1 = random(0,10);  //New name randomized in parts to make publishing it more manageable TODO this is a hacky way to do this prolly should be fixed
          int newNamePart2 = random(0,10);
          int newNamePart3 = random(1,10);
          int newName = newNamePart1 * 100 + newNamePart2 * 10 + newNamePart3;  //"Put together" the new randomized ID (just a integer between 1 and 1000)
          #if IDENTITYDEBUG
          int newNameDebug = newName;
          #endif
          for(int i = 0; i < 4; i++){        //Convoluted way to write new identity to the EEPROM
            if(newName - 255 > 0){           //Basically we have 4 bytes of storage available so we just "fill" the 4 bytes with the ID.
              EEPROM.write(NAMELOC+i, 255);  //Example: new randomized ID is 756. Which means 2 first bytes are 255 and the 3rd is 246. (255+255+246=756)
              EEPROM.commit();
              newName = newName - 255;
              }
            else{
              EEPROM.write(NAMELOC+i, newName);
              EEPROM.commit();
              newName = 0;
              }
            }
          #if IDENTITYDEBUG
          Serial.print("No previous name found, renamed itself to:");
          Serial.print(EEPROM.read(NAMELOC));
          Serial.print(" + ");
          Serial.print(EEPROM.read(NAMELOC+1));
          Serial.print(" + ");
          Serial.print(EEPROM.read(NAMELOC+2));
          Serial.print(" + ");
          Serial.print(EEPROM.read(NAMELOC+3));
          Serial.print(" = ");
          Serial.println(newNameDebug);
          #endif

          mqttClient.unsubscribe(topicString); //unsub from accepted topic
          
          if(topicString[l-1] == 49){    //Convoluted way to reduce the "empty" topics from the topicString to publish the new name to appropriate topic.
            topicString[l-1] = 57;
            if(topicString[l-2] == 48){
              topicString[l-2] = 57;
              topicString[l-3] = topicString[l-3] - 1;
              }
            else{
              topicString[l-2] = topicString[l-2] - 1;  
              }
            }
          else if(topicString[l-1] == 48){
            topicString[l-1] = 56;
            if(topicString[l-2] == 48){
              topicString[l-2] = 57;
              topicString[l-3] = topicString[l-3] - 1;
              }
            else{
              topicString[l-2] = topicString[l-2] - 1;  
              }
            }
          else{
            topicString[l-1] = topicString[l-1] - (emptyTopicCount-1);
            }
            
          char newNamePayload[4] = "000";  //Constructing payload for publishing the new name
          newNamePayload[0] = newNamePart1 + 48;
          newNamePayload[1] = newNamePart2 + 48;
          newNamePayload[2] = newNamePart3 + 48;
          #if IDENTITYDEBUG
          Serial.print("Publishing new name to: ");
          Serial.println(topicString);
          Serial.print("With the name: ");
          Serial.println(newNamePayload);
          #endif
          int nameHold = 0;
          nameHold = EEPROM.read(NAMELOC);  //Read ID in the nodes memory
          nameHold += EEPROM.read(NAMELOC+1);
          nameHold += EEPROM.read(NAMELOC+2);
          nameHold += EEPROM.read(NAMELOC+3);
          nodeNameFound = nameHold;
          mqttClient.publish(topicString, 2, 1, newNamePayload, 14); //Publish new name to appropriate topic          
          return;
          }
        }
      }
    }
  }
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#ifndef MAIN
void connectToWifi() {
  #if MQTTDEBUG
  Serial.println("Connecting to Wi-Fi...");
  #endif
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  #if MQTTDEBUG
  Serial.println("Connected to Wi-Fi.");
  #endif
  connectToMqtt();
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  #if MQTTDEBUG
  Serial.println("Disconnected from Wi-Fi.");
  #endif
  mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  wifiReconnectTimer.once(2, connectToWifi);
}

void connectToMqtt() {
  #if MQTTDEBUG
  Serial.println("Connecting to MQTT...");
  #endif
  mqttClient.connect();
}

void onMqttConnect(bool sessionPresent) {
  #if MQTTDEBUG
  Serial.println("Connected to MQTT.");
  Serial.print("Session present: ");
  Serial.println(sessionPresent);
  #endif
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  #if MQTTDEBUG
  Serial.println("Disconnected from MQTT.");
  #endif
  if (WiFi.isConnected()) {
    mqttReconnectTimer.once(2, connectToMqtt);
  }
}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  #if MQTTDEBUG
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
  #endif
}

void onMqttUnsubscribe(uint16_t packetId) {
  #if MQTTDEBUG
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  #endif
}

void onMqttPublish(uint16_t packetId) {
  #if MQTTDEBUG
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  #endif
}
#endif
