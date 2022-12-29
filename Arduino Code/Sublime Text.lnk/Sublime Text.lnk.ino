 #include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include <Ultrasonic.h>

bool manualmode=false;

Ultrasonic ultrasonic(D1, D2);
//Our Wi-Fi ssdid and password
char* ssid = "robosa";
char* password = "mahalaxmi";
String motor_cont = "0";

int count=0;
int pmillis=0;
int cmillis=0;

int modeswitch=A0;

int levelpercentage=0;

ESP8266WebServer server; //server variable

void setup() {
  initializePin(); //call function

  //Making Connection With netword
  WiFi.begin(ssid, password);
 
  Serial.begin(115200);
  Serial.print("Searching Connection");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
 
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.print(WiFi.localIP());
 
  serverSection();
}

void loop() {
if(analogRead(modeswitch)>980)
{
  manualmode=true;
}
else
{
  manualmode=false;
}

levelpercentage=ultrasonic.distanceRead();
//Serial.println(levelpercentage);
levelpercentage=constrain(levelpercentage,3,30);
count=map(levelpercentage,3,30,100,0);

if(manualmode==true)
{
  if(count<30 && motor_cont=="0")
  {
   motor_cont_state();
  }
  if(count>80 && motor_cont=="1")
  {
    motor_cont_state();
  }
}
  server.handleClient();
}

void initializePin(){
 
  pinMode(D5, OUTPUT);
  pinMode(modeswitch,INPUT);
 

  digitalWrite(D5, HIGH);

}

void serverSection(){
  server.on("/", []() {
    server.send(200, "text/html", "<!DOCTYPE html><html><meta charset='UTF-8'><head></head><body><h2>IOT Motor Control</h2><h3><a href='/motor_cont'>Motor Control</a></h3><br><h3><a href='/level_data'>level data</a></h3><br></body></html>");
  });

  server.on("/motor_cont", motor_cont_state);
   server.on("/level_data", get_level);


  server.on("/status", all_state);
 
  server.begin();
}

void motor_cont_state(){
  if(motor_cont == "0"){
    motor_cont = "1";
    digitalWrite(D5, LOW);
    server.send(200, "text/html", motor_cont);
    Serial.println("Received on");
  }else{
    motor_cont = "0";
    digitalWrite(D5, HIGH);
    server.send(200, "text/html", motor_cont);
  }
}


void get_level(){
 
    server.send(200, "text/html", String(count));
   
}


void all_state(){
 
  server.send(200, "text/html", "{\"rl\":\""+motor_cont+"\",\"lv\":\""+String(count)+"\"}");

}

