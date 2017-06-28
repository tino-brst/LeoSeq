#include "Arduino.h"
#include "Tools.h"

// Metodos p/debugging ========================================================

void printByte(byte b) {
   for (int i = 7; i >= 0; i--) {
      if (bitRead(b,i)) {
         Serial.print("1");
      } else {
         Serial.print("0");
      }
   }
   Serial.println(" ");
}
