//BOAT CODE:

#include <WiFi.h>
#include <WiFiUdp.h>
#include <ESP32Servo.h>

const char* ssid     = "ESP32_Controller";
const char* password = "12345678";

WiFiUDP udp;
const int udpPort = 4210;
char incoming[32];

const int IN1_PIN = 26;
const int IN2_PIN = 27;

const int SERVO_PIN = 13;
Servo rudder;

const int Y_REV_LIMIT = 1400;
const int Y_FWD_LIMIT = 2050;

// Steering ranges
const int X_RIGHT_MIN   = 1100;
const int X_LEFT_MAX    = 2600;

void setup() {
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);

  rudder.setPeriodHertz(50);
  rudder.attach(SERVO_PIN, 500, 2400);
  rudder.write(90);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(100); }

  udp.begin(udpPort);
}

void loop() {
  int packetSize = udp.parsePacket();
  if (!packetSize) return;

  int len = udp.read(incoming, sizeof(incoming) - 1);
  if (len <= 0) return;
  incoming[len] = '\0';

  int yValue = 0;
  int xValue = 0;
  sscanf(incoming, "%d,%d", &yValue, &xValue);

  // Throttle
  if (yValue >= Y_FWD_LIMIT) {
    digitalWrite(IN1_PIN, HIGH);
    digitalWrite(IN2_PIN, LOW);
  }
  else if (yValue <= Y_REV_LIMIT) {
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, HIGH);
  }
  else {
    digitalWrite(IN1_PIN, LOW);
    digitalWrite(IN2_PIN, LOW);
  }

  // Steering
  int angle = map(xValue, X_RIGHT_MIN, X_LEFT_MAX, 45, 135);
  rudder.write(constrain(angle, 45, 135));
}