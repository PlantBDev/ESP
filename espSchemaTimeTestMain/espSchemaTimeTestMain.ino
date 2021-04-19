#include <WiFi.h> //client for handling wifi connectivity
#include <WiFiUdp.h> // library for User Datagram Protocol handling
#include "netInfo.h" //information for wifi and NTP server
#include "eventTimer.h"
#define debugNTP 0

void setup() {

  Serial.begin(9600);
  Serial.print("Connecting to ");
  Serial.println(formSsid);
  WiFi.begin(formSsid, formPswd); //connect to the wifi network
  while (WiFi.status() != WL_CONNECTED) //print dots for as long as not connected
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); //Get and print IP from WiFi client
  //TODO check if timeClient is already running
  timeClient.begin();//start timeClient with the setup specified in NTPinfo.h
  timeClient.setTimeOffset(timeOffset); //converts the time from UTC to local(defined in NTPinfo.h)
}

void loop() {
  int *ClockOutputTest; //test variable for testing NTPclock output. NTPclock returns a pointer, so this is a pointer to an integer
  int testHours = 0;
  int testMinutes = 0;
  ClockOutputTest = NTPclock();

  //testHours = *ClockOutputTest; //NTPclock returns a pointer to 2 place array first hours then minutes
  //testMinutes = *(ClockOutputTest + 1);
  //Serial.print(testHours);
  //Serial.println(testMinutes);
  eventTimer(12, 20, 20, 50, 50, 50, 50);


  
  delay(10000);

}
