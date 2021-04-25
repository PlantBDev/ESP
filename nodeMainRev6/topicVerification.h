//compares two character strings that are given to the function as input and returns TRUE or FALSE depending on the results
//third parametre determines how many characters are compared from the string


bool topicVerification(char topic[21], char compare[21], int times){
 char topicVerification[times];  //used to verificate what was the topic where the publish was made
 char compareVerification[times];
 strncpy(topicVerification,topic,21);
 strncpy(compareVerification,compare,21);
 
 for(int i=0 ; i<times ; i++){
   if(topicVerification[i] != compareVerification[i]){
     return 0;
   }
 } 
 return 1;
}
