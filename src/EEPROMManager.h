#ifndef EEPROMMANAGER_H
#define EEPROMMANAGER_H

#include <Arduino.h>

void eepromBegin();
void eepromEnd();
void eepromWriteString(int startAddress, const String& str, int maxLength);
String eepromReadString(int startAddress, int maxLength);

#endif
