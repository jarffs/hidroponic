#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <Arduino.h>

void loadWiFiCredentials(String& ssid, String& password);
void saveWiFiCredentials(const String& ssid, const String& password);
bool connectWiFi(const String& ssid, const String& password);
void startAP();

#endif
