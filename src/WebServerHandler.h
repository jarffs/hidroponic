#ifndef WEBSERVERHANDLER_H
#define WEBSERVERHANDLER_H

#include <ESP8266WebServer.h>

extern ESP8266WebServer server;
extern unsigned long motorInterval;
extern unsigned long ledInterval;

void setupServer();
void handleRoot();
void handleOn();
void handleLedOn();
void handleLedOff();
void handleOff();
void handleWiFiSave();
void handleSpeed();
void handleMotorInterval();
void handleLedInterval();

#endif
