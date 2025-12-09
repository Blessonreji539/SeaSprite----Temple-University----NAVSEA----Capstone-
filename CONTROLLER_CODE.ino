//This is NOT Python, its Arduino C++.
//CONTROLLER CODE:

#include <WiFi.h>
#include <WiFiUdp.h>

// Wifi network name  for controller
const char* ssid     = "ESP32_Controller";
const char* password = "12345678";

WiFiUDP udp;
const int udpPort = 4210;

IPAddress boatIP(192, 168, 4, 255);

// joystick input pins
const int Y_AXIS_PIN = 34;
const int X_AXIS_PIN = 32;

void setup() {
  WiFi.softAP(ssid, password);
  udp.begin(udpPort);
}

void loop() {
  int yRaw = analogRead(Y_AXIS_PIN);
  int xRaw = analogRead(X_AXIS_PIN);

  udp.beginPacket(boatIP, udpPort);
  udp.print(yRaw);
  udp.print(",");
  udp.print(xRaw);
  udp.endPacket();

  delay(30);
}