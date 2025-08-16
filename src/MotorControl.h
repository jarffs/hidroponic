#ifndef MOTORCONTROL_H
#define MOTORCONTROL_H

#include <Arduino.h>

void setupMotors();
void turnMotorsOn();
void turnMotorsOff();
void setMotorSpeed(int speed);
bool areMotorsOn();

#endif
