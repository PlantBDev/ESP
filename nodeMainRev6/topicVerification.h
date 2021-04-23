//compares two character strings that are given to the function as input and returns TRUE or FALSE depending on the results


bool topicVerification(char topic[21], char compare[21]){
 char topicVerification[21] = "00000000000000000000";  //used to verificate what was the topic where the publish was made
 char compareVerification[21] = "00000000000000000000";
 strncpy(topicVerification,topic,21);
 strncpy(compareVerification,compare,21);
 
 if(nodeNameFound == 0){
  for(int i=0 ; i<10 ; i++){
    if(topicVerification[i] != compareVerification[i]){
      return 0;
    }
  }
 }
 else{
  for(int i=0 ; i<21 ; i++){
    if(topicVerification[i] != compareVerification[i]){
      return 0;
    }
  }
 } 
 return 1;
}
