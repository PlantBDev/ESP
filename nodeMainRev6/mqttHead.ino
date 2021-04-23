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
  #if MQTT_MSG_DEBUG
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


  char outputChar[10];  //Conversions tool for the sensor data, because "mqttPub" eats only character strings
  static uint16_t deviceId;
  int payloadComb;
  
  char topicCompare[21] = "node/Count"; //Topics to compare with received topic
  char sensorCompare[21] = "node/id";
  char dangerCompare[21] = "node/danger/id";
  char serverIdCompare[21] = "server/id";



  //         For Node Identification         //
  if(topicVerification(topic,topicCompare)){  
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

  if(topicVerification(topic,sensorCompare)){
    payloadComb = (payload[0]-48) * 100 + (payload[1]-48) * 10 + (payload[2]-48);  //MAek integer from the payload to make comparing ID in devices memory and in the payload easier  
    if(nodeNameFound == payloadComb){     //if the device ID and payloads ID match sensor readings can be sent
     #if SENSORDEBUG
     Serial.println("Sensor readings requested!");
     #endif

     // Sensor readings publish
     String str;

     if(temperature != 888888888){
       str=String(temperature);
       str.toCharArray(outputChar,10);
       mqttPub(outputChar, "node/sensor/temp");
     }
     else{
       #if SENSORDEBUG
       Serial.println("Temp reading not published, sensor value isn't alright, you should check if sensor is ok :(");
       #endif  
     }
     if(humidity != 888888888){
       str=String(humidity);
       str.toCharArray(outputChar,10);
       mqttPub(outputChar, "node/sensor/hum");
     }
     else{
       #if SENSORDEBUG
       Serial.println("Hum reading not published, sensor value isn't alright, you should check if sensor is ok :(");
       #endif  
     }
     if(soilMoisture != 888888888){
       str=String(soilMoisture);
       str.toCharArray(outputChar,10);
       mqttPub(outputChar, "node/sensor/moist");
     }
     else{
       #if SENSORDEBUG
       Serial.println("Moisture reading not published, sensor value isn't alright, you should check if sensor is ok :(");
       #endif  
     }
     if(light != 888888888){
       str=String(light);
       str.toCharArray(outputChar,10);
       mqttPub(outputChar, "node/sensor/light");
     }
     else{
       #if SENSORDEBUG
       Serial.println("Light reading not published, sensor value isn't alright, you should check if sensor is ok :(");
       #endif  
     }
      #if SENSORDEBUG
      Serial.println("Sensor readings published");
      #endif
    }
    else{
     #if SENSORDEBUG
     Serial.println("Node ID != payload ID\t");
     Serial.print(nodeNameFound);
     Serial.print(" != ");
     Serial.print(payloadComb);
     #endif 
    }
  }


  
  //          For Danger Topic Publish Requests          //
  if(topicVerification(topic,dangerCompare)){
    payloadComb = (payload[0]-48) * 100 + (payload[1]-48) * 10 + (payload[2]-48);  //MAek integer from the payload to make comparing ID in devices memory and in the payload easier  
    if(nodeNameFound == payloadComb){  //Confirm that message had matching id with nodes id
      #if SENSORDEBUG
      Serial.println("Danger statuses requested!");
      #endif
      outputChar[0] = tempSensorBroke+48;
      mqttPub(outputChar, "node/danger/reading/temp");  //Publish to node/danger/# topics
      outputChar[0] = humiditySensorBroke+48;
      mqttPub(outputChar, "node/danger/reading/hum");
      outputChar[0] = moistureSensorBroke+48;
      mqttPub(outputChar, "node/danger/reading/moist");
      outputChar[0] = lightSensorBroke+48;
      mqttPub(outputChar, "node/danger/reading/light");
      outputChar[0] = waterLevelSensorBroke+48;
      mqttPub(outputChar, "node/danger/actuator/waterPump");
      #if SENSORDEBUG
      Serial.println("Danger statuses published");
      #endif
    }
    else{
     #if DANGERDEBUG
     Serial.println("Node ID != payload ID\t");
     Serial.print(nodeNameFound);
     Serial.print(" != ");
     Serial.print(payloadComb);
     #endif 

    }
  }
  if(topicVerification[0] == serverIdCompare[0] && topicVerification[1] == serverIdCompare[1] && topicVerification[2] == serverIdCompare[2] && topicVerification[3] == serverIdCompare[3] && topicVerification[4] == serverIdCompare[4] && topicVerification[5] == serverIdCompare[5] && topicVerification[6] == serverIdCompare[6] && topicVerification[7] == serverIdCompare[7] && topicVerification[8] == serverIdCompare[8] && topicVerification[9] == serverIdCompare[9] && topicVerification[10] == serverIdCompare[10]){
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
      schematicProgress++;
      Serial.println("message recieved ;)");
      #if MQTTDEBUG
        Serial.print("schematicProgress is ");
        Serial.print(schematicProgress);
      #endif
    }
  }
    if(schematicProgress)
    {
      if(schematicProgress <= 7)
      {
        if(schematicProgress >= 2)
        {
          schematicArray[schematicProgress - 2] = (payload[0]-48) * 100 + (payload[1]-48) * 10 + (payload[2]-48);  //MAek integer from the payload and store the value to the array
          #if MQTTDEBUG
            Serial.print("Stored payload: ");
            Serial.print(schematicArray[schematicProgress - 2]);
            Serial.print(" and stored it in array place: ");
            Serial.println(schematicProgress - 2);
          #endif
          if(schematicProgress == 7) //this cant be done after the incrementation because there would be no new message that would trigger this event.
          {
            schematicProgress = 8; //to indicate for a check in main that it's safe to read variables from global array
            mqttClient.unsubscribe("server/humlightoff"); //this here for the same reason as the comment in the if statement above describes
          }
        }
        if(schematicProgress != 8)
        {
          schematicProgress++;
        }
        switch(schematicProgress - 1) //minus 1 because increment happens before we are here //old information //no longer old information
        {
          case 1:
          mqttClient.subscribe("server/lightonday", 2);
          #if MQTTDEBUG
              Serial.println("Subscribed to server/lightonday");
          #endif
          break;
          case 2:
            mqttClient.unsubscribe("server/lightonday");
            #if MQTTDEBUG
              Serial.println("unsubscribed from server/lightonday");
            #endif
            mqttClient.subscribe("server/templighton", 2);
            #if MQTTDEBUG
              Serial.println("Subscribed to server/templighton");
            #endif
            break;
          case 3:
            mqttClient.unsubscribe("server/templighton");
            mqttClient.subscribe("server/templightoff", 2);
            break;
          case 4:
            mqttClient.unsubscribe("server/templightoff");
            mqttClient.subscribe("server/moistlighton", 2);
            break;
          case 5:
            mqttClient.unsubscribe("server/moistlighton");
            mqttClient.subscribe("server/humlighton", 2);
            break;
          case 6:
            mqttClient.unsubscribe("server/humlighton");
            mqttClient.subscribe("server/humlightoff", 2);
            break;
          case 0:
            Serial.println("Subscribe to pewdiepie!");
          break;
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
