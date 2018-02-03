
#include <DHT.h>
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

#define DHTPIN 2     // what digital pin we're connected to (warning: IDE pins and ESP pins do not match)
#define DHTTYPE DHT11   // DHT 11


ESP8266WebServer server(80);
WiFiClient client;
DHT dht(DHTPIN, DHTTYPE);
 
void setup () {
    dht.begin();
    
#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif
  Serial.begin(9600);
  delay(3000); // wait for console opening




  // Connect to wifi
  Serial.print("Setting soft-AP ... ");
  WiFi.begin("ijp-79839", "pp3c-ld6f-wuy5-1fbx");
  Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  
    Serial.println("Connected!");
    Serial.println(WiFi.localIP());
  launchServer();
}

void launchServer() {
  Serial.println("Lauching web server...");
  server.on("/", sendOK);
  server.on("/get_temp", sendTemp);
  server.on("/get_hum", sendHum);
  server.on("/get_all", sendAll);
  
  server.begin();
  Serial.println("Web server ready!");
}

void sendOK() {
  server.send(200, "text/plain", "OK");
}

void sendTemp()
{
  int t = dht.readTemperature();
  if(t < 100 && !isnan(t)){
  server.send(200, "text/plain", String(t));
  }
  
}

void sendHum()
{  
  int h = dht.readHumidity();
  if(h < 100 && !isnan(h))
  {
    server.send(200, "text/plain", String(h));
  }
}

void sendAll()
{ 
  int t = dht.readTemperature();
  int h = dht.readTemperature();
  server.send(200, "text/plain", String(t)+';'+String(h));
}


void loop () {
    server.handleClient();

    int t = dht.readHumidity();

    Serial.println(t);
    delay(1000);
}
