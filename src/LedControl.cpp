#include "LedControl.h"
#include "MotorControl.h"
#include <Arduino.h>

const int ledB1 = D1;  // L298N IN3
const int ledB2 = D2;  // L298N IN4  
const int ledENB = D6; // L298N ENB (MUST be connected and enabled!)

static bool ledOn = false;

void setupLed() {
  pinMode(ledB1, OUTPUT);
  pinMode(ledB2, OUTPUT);
  pinMode(ledENB, OUTPUT);
  turnLedOff();
}

void turnLedOn() {
  // Check if motors are currently on
  if (areMotorsOn()) {
    Serial.println("WARNING: Cannot turn on LEDs - Motors are currently on. Turning off motors first.");
    turnMotorsOff();
    delay(500); // Small delay for safety
  }
  
  ledOn = true;
  digitalWrite(ledB1, HIGH);
  digitalWrite(ledB2, LOW);
  analogWrite(ledENB, 255);  // ESSENTIAL: Enable the output!
  Serial.println("DEBUG: LED on - B1=HIGH, B2=LOW, ENB=255");
}

void turnLedOff() {
  ledOn = false;
  digitalWrite(ledB1, LOW);
  digitalWrite(ledB2, LOW);
  analogWrite(ledENB, 0);     // Disable the output
  Serial.println("DEBUG: LED off - B1=LOW, B2=LOW, ENB=0");
}

bool areLedOn() {
  return ledOn;
}
