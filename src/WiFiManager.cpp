#include <ESP8266WiFi.h>
#include "WiFiManager.h"
#include "EEPROMManager.h"

#define EEPROM_SSID_ADDR 0
#define EEPROM_PASS_ADDR 32
#define EEPROM_MAX_LEN 32

void loadWiFiCredentials(String& ssid, String& password) {
  eepromBegin();
  ssid = eepromReadString(EEPROM_SSID_ADDR, EEPROM_MAX_LEN);
  password = eepromReadString(EEPROM_PASS_ADDR, EEPROM_MAX_LEN);
  eepromEnd();
}

void saveWiFiCredentials(const String& ssid, const String& password) {
  eepromBegin();
  eepromWriteString(EEPROM_SSID_ADDR, ssid, EEPROM_MAX_LEN);
  eepromWriteString(EEPROM_PASS_ADDR, password, EEPROM_MAX_LEN);
  eepromEnd();
}

bool connectWiFi(const String& ssid, const String& password) {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    attempts++;
  }
  return WiFi.status() == WL_CONNECTED;
}

void startAP() {
  WiFi.softAP("MotorControllerESP");
}
