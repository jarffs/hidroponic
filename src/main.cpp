#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "MotorControl.h"
#include "LedControl.h"
#include "WiFiManager.h"
#include "WebServerHandler.h"

unsigned long previousMotorMillis = 0;
unsigned long previousLedMillis = 0;
extern unsigned long motorInterval;
extern unsigned long ledInterval;

String ssid;
String password;

void setup() {
  Serial.begin(115200);

  setupMotors();
  setupLed();

  loadWiFiCredentials(ssid, password);

  bool connected = connectWiFi(ssid, password);
  if (connected) {
    Serial.println("Conectado! IP: " + WiFi.localIP().toString());
  } else {
    Serial.println("Falha ao conectar. Criando AP...");
    startAP();
    Serial.println("Acesse: http://192.168.4.1");
  }

  setupServer();
}

void loop() {
  server.handleClient();

  unsigned long currentMillis = millis();
  
  // Handle motor interval (motors have priority over LEDs)
  if (currentMillis - previousMotorMillis >= motorInterval) {
    previousMotorMillis = currentMillis;
    if (areMotorsOn()) {
      turnMotorsOff();
      Serial.println("Motors turned off automatically.");
    } else {
      // Only turn on motors if LEDs are not on, or turn off LEDs first
      if (areLedOn()) {
        turnLedOff();
        Serial.println("LEDs turned off to allow motors to turn on.");
        delay(500);
      }
      turnMotorsOn();
      Serial.println("Motors turned on automatically.");
    }
  }
  
  // Handle LED interval (only if motors are not on)
  if (currentMillis - previousLedMillis >= ledInterval) {
    previousLedMillis = currentMillis;
    if (areLedOn()) {
      turnLedOff();
      Serial.println("LED turned off automatically.");
    } else if (!areMotorsOn()) {
      // Only turn on LEDs if motors are not running
      turnLedOn();
      Serial.println("LED turned on automatically.");
    } else {
      Serial.println("LED auto-turn-on skipped - Motors are currently running.");
    }
  }
}
