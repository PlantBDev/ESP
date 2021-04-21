// Simple function for simplefying already very simple mqtt publishing
// Called with => mqttPub(message, topic);
// Message and topic should be character strings with max length of 50
// This header should be included AFTER instantiating the AsyncMqttClient object
//  otherwise there will be errors about mqttClient not being defined.

#ifndef MAIN
#define MQTTPUBDEBUG 1 // Prints about puplished messages
#endif

void mqttPub(char message[50],char topic[50]){  //TODO add parametre for the retain flag
 mqttClient.publish(topic, 2, false, message); //explained below
 //publish( parametres below )         
 //                 const char* topic, (self explanatory)
 //                 uint8_t qos, (quality of service 0,1 or 2)
 //                 bool retain, (determines if the message is retained in the broker)
 //                 const char* payload = nullptr, ("message" to be sent)
 //                 size_t length = 0, (length of the message, will be automatically calculated if not set)
 //                 bool dup = false, (duplicate flag)
 //                 uint16_t message_id = 0) (id for the message can be used to check for duplicates)
                      
 #if MQTTPUBDEBUG
 Serial.printf("Publishing on topic %s at QoS 2", topic);
 Serial.print("\n");
 Serial.print("Message: ");
 Serial.println(message);
 #endif
}
