#include "MotorControl.h"
#include "LedControl.h"
#include <Arduino.h>

const int motorA1 = D3;
const int motorA2 = D4;
const int motorENA = D5;

static bool motorsOn = false;
static int motorSpeed = 1023;  // PWM máximo

void setupMotors() {
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorENA, OUTPUT);
  turnMotorsOff();
}

void turnMotorsOn() {
  // Check if LEDs are currently on
  if (areLedOn()) {
    Serial.println("WARNING: Cannot turn on motors - LEDs are currently on. Turning off LEDs first.");
    turnLedOff();
    delay(500); // Small delay for safety
  }
  
  motorsOn = true;
  digitalWrite(motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  analogWrite(motorENA, motorSpeed);
  Serial.println("DEBUG: Motors turned on - A1=HIGH, A2=LOW, ENA=" + String(motorSpeed));
}

void turnMotorsOff() {
  motorsOn = false;
  digitalWrite(motorA1, LOW);
  digitalWrite(motorA2, LOW);
  analogWrite(motorENA, 0);
  Serial.println("DEBUG: Motors turned off - A1=LOW, A2=LOW, ENA=0");
}

void setMotorSpeed(int speed) {
  motorSpeed = constrain(speed, 0, 255);
  if (motorsOn) {
    analogWrite(motorENA, motorSpeed);
  }
}

bool areMotorsOn() {
  return motorsOn;
}
