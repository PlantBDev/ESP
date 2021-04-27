//          For Danger Topic Publish            //

void dangerPublish(){
 String str;
 char outputChar[10] = {'\0','\0','\0','\0','\0','\0','\0','\0','\0'};
 #if SENSORDEBUG
 Serial.println("Danger statuses changed!");
 #endif
 outputChar[0] = tempSensorBroke+48;
 mqttClient.publish("node/danger/reading/temp", 2, true, outputChar); //Publish to node/danger/# topics
 outputChar[0] = humiditySensorBroke+48;
 mqttClient.publish("node/danger/reading/hum", 2, true, outputChar);
 outputChar[0] = moistureSensorBroke+48;
 mqttClient.publish("node/danger/reading/moist", 2, true, outputChar);
 outputChar[0] = lightSensorBroke+48;
 mqttClient.publish("node/danger/reading/light", 2, true, outputChar);
 outputChar[0] = waterLevelSensorBroke+48;
 mqttClient.publish("node/danger/reading/waterPump", 2, true, outputChar);
 str=String(nodeNameFound);
 str.toCharArray(outputChar,10);
 mqttClient.unsubscribe("node/danger/id");
 mqttClient.publish("node/danger/id", 2, true, outputChar); //TODO figure how to make this be retained without node going to unlimited loop
 mqttClient.subscribe("node/danger/id", 2);
 for(int i=0; i<10;i++){  //Setting all of outputChars values to null to make sure the string terminating character is in correct place
  outputChar[i] = '\0';
 }

 #if SENSORDEBUG
 Serial.println("Danger statuses published");
 #endif
}
