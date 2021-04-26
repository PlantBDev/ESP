//compares two character strings that are given to the function as input and returns TRUE or FALSE depending on the results
//third parametre determines how many characters are compared from the string


bool topicVerification(char topic[21], char compare[21], int times){
//Serial.println("anteeksi mita vittua"); // if this is added the code works...
 char topicVerification[times];  //used to verificate what was the topic where the publish was made
 char compareVerification[times];
 strncpy(topicVerification,topic,times); //changed from 21 to times variable
 strncpy(compareVerification,compare,times);
 
 for(int i=0 ; i<times ; i++){
   if(topicVerification[i] != compareVerification[i]){
     return 0;
   }
 } 
 return 1;
}
