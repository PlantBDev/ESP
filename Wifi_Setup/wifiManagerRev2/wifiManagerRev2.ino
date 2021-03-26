#ifndef MAIN
#ifndef WIFIMANDEBUG
#define WIFIMANDEBUG 1
#endif



// TODO include these to the main Scketckh?//
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
IPAddress local_ip(192,168,1,1);   //TODO May need to change these things probably need to ask for help!
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0); 
ESP8266WebServer server(80);
String formSsid;
String formPswd;
/////////////////////////////////////////////



void setup(){
  Serial.begin(115200);
}
void loop(void){
  wifiManager();
  /*#if WIFIMANDEBUG
  Serial.println("Success");
  Serial.println(formSsid);
  Serial.println(formPswd);
  #endif*/
}
#endif



//WifiManager function TODO get rid of gloabal variables "formSsid" and "formPswd"
void wifiManager(){
  const char* apSsid = "NodeMCU";       //Access point variable setup. 
  const char* apPassword = "12345678";  //TODO access points default Ssid and password could need some consideration maybe?
  
  formSsid = rand();    //Randomize the "form" variables and store these randomized value to "safety" variables which are used
  formPswd = rand();    //at the end of this function to make sure not to end it before both Ssid and password are given.
  String safetySsid = formSsid; 
  String safetyPswd = formPswd;
  
  WiFi.softAP(apSsid, apPassword); //Engages soft accespoint mode with first parametre being the access points Ssid and other being the password.
                                          
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
  
  #if WIFIMANDEBUG   //Debug prints
  Serial.println("");
  Serial.println("Access point mode");
  Serial.print("SSID: ");
  Serial.println(apSsid);
  Serial.print("Pswd: ");
  Serial.println(apPassword);
  Serial.print("IP address: ");
  Serial.println(local_ip);  //IP address for the web server
  #endif
  
  server.on("/", handleRoot);            //Which routine to handle at root location
  server.on("/handle_form", handleForm); //form action is handled in the "handleForm" function
  server.onNotFound(handle_NotFound);    //404 handle right here just for me

  server.begin();                  //Start server
  #if WIFIMANDEBUG
  Serial.println("HTTP server started");
  #endif

  while( formSsid == safetySsid || formPswd == safetyPswd){  //Loop which requires both Ssid and password to be changed to proceed
    server.handleClient(); //Handle client site requests
  }
}


//////////////////////////
// Page request handles //
//////////////////////////

void handleRoot() {     //Sends root webpage, TODO html string could be formatted to be more readable
 server.send(200, "text/html", "<!DOCTYPE html><html><body><h2>WIFI Setup<h2><h3> Input your Wifi SSID and password</h3><form action=\"/handle_form\" method=\"post\">SSID<br> <input type=\"text\" name=\"formSsid\" value=\"\"> <br> Password <br><input type=\"text\" name=\"pswd\" value=\"\"> <br><br> <input type=\"submit\" value=\"Submit\"></form></body></html>");
}

void handleForm() {                 //What happens when submit button is pressed and "/handle_form" site is requested
 formSsid = server.arg("formSsid"); //Assing info from html form to variables
 formPswd = server.arg("pswd"); 
 #if WIFIMANDEBUG  // Prints for testing
 Serial.print("ssid:");
 Serial.println(formSsid);
 Serial.print("password:");
 Serial.println(formPswd);
 #endif
 
 String s = "<a href='/'> Go Back </a>";
 server.send(200, "text/html", s); //Send "Go back" web page
}


void handle_NotFound(){               // Handle for 404 issue
  server.send(404, "text/plain", "Not found");
} 
