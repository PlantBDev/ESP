  #include <NTPClient.h> //library for network time protocol handling
  #include <WiFiUdp.h> // library for User Datagram Protocol handling
  #include "NTPInfo.h" //header that stores ntp server information
  //github page for ntp client
  //https://github.com/arduino-libraries/NTPClient
  
  //------------------------------------Debug printting enabled via defining debugNTP as 1----------------------------
  //if server used to query for time crashes, timeClient returns 6H28M UTC for some reason. It should not return anything and just hang but it doesnt.
  
  //DONE create check if UDP class is created already and if NTPclient is setup already so that these could be moved to a header
  WiFiUDP ntpUDP; //Name for udp class
  NTPClient timeClient(ntpUDP,"pool.ntp.org"); //setup the NTP client with UDP and NTP server address
  
  int * NTPclock()
  {
    //github page for ntp client
    //https://github.com/arduino-libraries/NTPClient 

    static int timeArr[2]; //array to return time in. Hours in first place and minutes in the second.
                           //Tihs has to be STATIC in order to be readable outside the context of this function
    //TODO what to do when NTP server stops responding or becomes unreachable
    while(!timeClient.update()) //Loop to retry until succesful update is recieved
    {
      timeClient.update(); //get updated time from NTP server
      delay(500);
    }
    timeArr[0] = timeClient.getHours();
    timeArr[1] = timeClient.getMinutes();
    #if debugNTP //test prints for ntp time
      Serial.print(timeArr[0]);
      Serial.print(" ");
      Serial.println(timeArr[1]);
    #endif
    return timeArr;
  }
