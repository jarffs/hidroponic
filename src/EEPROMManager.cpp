#include <EEPROM.h>
#include "EEPROMManager.h"

void eepromBegin() {
  EEPROM.begin(512);
}

void eepromEnd() {
  EEPROM.end();
}

void eepromWriteString(int startAddress, const String& str, int maxLength) {
  for (int i = 0; i < maxLength; i++) {
    if (i < str.length()) {
      EEPROM.write(startAddress + i, str[i]);
    } else {
      EEPROM.write(startAddress + i, 0);
    }
  }
  EEPROM.commit();
}

String eepromReadString(int startAddress, int maxLength) {
  char buff[maxLength + 1];
  for (int i = 0; i < maxLength; i++) {
    buff[i] = EEPROM.read(startAddress + i);
  }
  buff[maxLength] = '\0';
  return String(buff);
}
