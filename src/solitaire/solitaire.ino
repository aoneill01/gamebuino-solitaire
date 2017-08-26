#include <SPI.h>
#include <Gamebuino.h>
#include <EEPROM.h>

Gamebuino gb;

void setup() {
  gb.begin();
}

void loop() {
  if (gb.update()) {
  
  }
}