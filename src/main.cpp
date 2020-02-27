#include <WiFiUdp.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>

const char* ssid = "Faraday";
const char* password = "Parola1905*";
IPAddress ServerIP(192,168,1,33);
IPAddress ClientIP(192,168,1,64);

WiFiUDP Udp;
unsigned int localUdpPort = 4210;  // local port to listen on
char incomingPacket[255];  // buffer for incoming packets
char  ledon[] = "ledi yaktim";
char ledoff[] = "ledi sondurdum";


void setup()
{
  Serial.begin(115200);
  Serial.println();
  pinMode(15,OUTPUT);
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  Serial.println(WiFi.localIP());

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
}


void loop()
{
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);
    if (len > 0)
    {
      incomingPacket[len] = 0; 
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket); 
    if(0 == strcmp(incomingPacket,"ON"))
    {
      digitalWrite(15,HIGH);
      Udp.beginPacket(ServerIP, localUdpPort);
      Udp.write(ledon);
      Udp.endPacket(); 
    }
    else if(0 == strcmp(incomingPacket,"OFF"))
    {
      digitalWrite(15,LOW);
      Udp.beginPacket(ServerIP, localUdpPort);
      Udp.write(ledoff);
      Udp.endPacket();
    }
   
  }
}