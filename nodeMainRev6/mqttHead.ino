//Main library used for mqtt "AsyncMqttClient"
//Source: https://github.com/marvinroger/async-mqtt-client

void mqttSetup(){
  mqttconnected = 0;
  wificonnected = 0;
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
  
  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onSubscribe(onMqttSubscribe);
  mqttClient.onMessage(onMqttMessage);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_IP, MQTT_PORT);
}




/////////////////////////////////////////////////////////
//                     OnMqttMessage                   //
/////////////////////////////////////////////////////////
void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {  //This function is called everytime there is a new publish to a subscibed MQTT topic
  #if MQTTDEBUG
  Serial.println("Publish received.");
  Serial.print("  topic: ");
  Serial.println(topic);
  Serial.print("  qos: ");
  Serial.println(properties.qos);
  Serial.print("  dup: ");
  Serial.println(properties.dup);
  Serial.print("  retain: ");
  Serial.println(properties.retain);
  Serial.print("  len: ");
  Serial.println(len);
  Serial.print("  index: ");
  Serial.println(index);
  Serial.print("  total: ");
  Serial.println(total);
  Serial.print("Payload: ");
  Serial.println(payload);
  #endif
  
  static uint16_t deviceId;
  int payloadComb;

  char topicVerification[11] = "0000000000";  //used to verificate what was thr topic where the publish was made
  
  char topicCompare[11] = "node/Count"; //Topics to compare with received topic
  char sensorCompare[11] = "node/id";

  strncpy(topicVerification,topic,10);
  
  //         For Node Identification         //
  //if(topicVerification == topicCompare){ //TODO WHY DOES THIS NOT WORK AND LINE BELOW DOES?
  if(topicVerification[0] == topicCompare[0] && topicVerification[1] == topicCompare[1] && topicVerification[2] == topicCompare[2] && topicVerification[3] == topicCompare[3] && topicVerification[4] == topicCompare[4] && topicVerification[5] == topicCompare[5] && topicVerification[6] == topicCompare[6] && topicVerification[7] == topicCompare[7] && topicVerification[8] == topicCompare[8] && topicVerification[9] == topicCompare[9] && topicVerification[10] == topicCompare[10]){
    nameCount[0] = nameCount[0]+1;  //Incrementing nameCount to keep track of topics with something in them
    deviceId = EEPROM.read(NAMELOC);  //Read ID in the nodes memory
    deviceId += EEPROM.read(NAMELOC+1);
    deviceId += EEPROM.read(NAMELOC+2);
    deviceId += EEPROM.read(NAMELOC+3);
    payloadComb = (payload[0]-48) * 100 + (payload[1]-48) * 10 + (payload[2]-48);  //MAek integer from the payload to make comparing ID in devices memory and in the payload easier
    #if IDENTITYDEBUG
    Serial.print("Nodes ID in memory: ");
    Serial.print(EEPROM.read(NAMELOC));
    Serial.print(" + ");
    Serial.print(EEPROM.read(NAMELOC+1));
    Serial.print(" + ");
    Serial.print(EEPROM.read(NAMELOC+2));
    Serial.print(" + ");
    Serial.print(EEPROM.read(NAMELOC+3));
    Serial.print(" = ");
    Serial.println(deviceId);
    Serial.print("Topic payload value: ");
    Serial.print((payload[0]-48)*100);
    Serial.print(" + ");
    Serial.print((payload[1]-48)*10);
    Serial.print(" + ");
    Serial.print((payload[2])-48);
    Serial.print(" = ");
    Serial.println(payloadComb);
    #endif
    if(deviceId == payloadComb){  //Confirm that matching ID was found
      nodeNameFound = deviceId;
    }
  }


  //          For Sensor publish Requests          //
  //TODO below is a stupid way to do this, should be fixed
  if(topicVerification[0] == sensorCompare[0] && topicVerification[1] == sensorCompare[1] && topicVerification[2] == sensorCompare[2] && topicVerification[3] == sensorCompare[3] && topicVerification[4] == sensorCompare[4] && topicVerification[5] == sensorCompare[5] && topicVerification[6] == sensorCompare[6] && topicVerification[7] == sensorCompare[7] && topicVerification[8] == sensorCompare[8] && topicVerification[9] == sensorCompare[9] && topicVerification[10] == sensorCompare[10]){
    deviceId = EEPROM.read(NAMELOC);  //Read ID in the nodes memory
    deviceId += EEPROM.read(NAMELOC+1);
    deviceId += EEPROM.read(NAMELOC+2);
    deviceId += EEPROM.read(NAMELOC+3);
    payloadComb = (payload[0]-48) * 100 + (payload[1]-48) * 10 + (payload[2]-48);  //MAek integer from the payload to make comparing ID in devices memory and in the payload easier
    #if MQTTPUBDEBUG
    Serial.print("Nodes ID in memory: ");
    Serial.print(EEPROM.read(NAMELOC));
    Serial.print(" + ");
    Serial.print(EEPROM.read(NAMELOC+1));
    Serial.print(" + ");
    Serial.print(EEPROM.read(NAMELOC+2));
    Serial.print(" + ");
    Serial.print(EEPROM.read(NAMELOC+3));
    Serial.print(" = ");
    Serial.println(deviceId);
    Serial.print("Topic payload value: ");
    Serial.print((payload[0]-48)*100);
    Serial.print(" + ");
    Serial.print((payload[1]-48)*10);
    Serial.print(" + ");
    Serial.print((payload[2])-48);
    Serial.print(" = ");
    Serial.println(payloadComb);
    #endif
    if(deviceId == payloadComb){  //if the device ID and payloads ID match sensor readings can be sent
     #if SENSORDEBUG
     Serial.println("Sensor readings requested!");
     #endif
     if(temperature != 888888888 && humidity != 888888888 && soilMoisture != 888888888 && light != 888888888){
      // Sensor readings publish
      char outputChar[10];  //Conversions for the sensor data, because "mqttPub" eats only character strings
      String str;
      
      str=String(temperature);
      str.toCharArray(outputChar,10);
      mqttPub(outputChar, "node/sensor/temp");  //TODO find out correct topics for these
      
      str=String(humidity);
      str.toCharArray(outputChar,10);
      mqttPub(outputChar, "node/sensor/hum");
      
      str=String(soilMoisture);
      str.toCharArray(outputChar,10);
      mqttPub(outputChar, "node/sensor/moist");
      
      str=String(light);
      str.toCharArray(outputChar,10);
      mqttPub(outputChar, "node/sensor/light");
      
      #if SENSORDEBUG
      Serial.println("Sensor readings published");
      #endif
      }
     else{
      #if SENSORDEBUG
      Serial.println("Sensor readings not published, sensor values aren't alright, you should check if sensor are ok :(");
      #endif 
      }
    } 
  }
}

void connectToWifi() {  //As the name implies try to connect to wifi with credentials from the wifiManager
  #if MQTT_PROT_DEBUG
  Serial.println("Connecting to Wi-Fi...");
  #endif
  WiFi.begin(formSsid, formPswd);
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
  mqttClient.connect();
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
