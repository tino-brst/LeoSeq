#include "Arduino.h"
#include "Tools.h"
#include "DriverLEDs.h"
#include "DriverSPShiftReg.h"

static const int regCount = 1;
static const int regWidth = 8;
static long currentPattern = 0;

static long blinkTime = 500;
static long checkPointBlink = 0;
static bool blinkON = true;

static long runUpdateTime = 75;
static long currentRunLED = 0;
static long checkPointRun = 0;

void LEDsInit() {
  spShiftRegInit(PB7, PD6, PC7, regCount);
}

void setLEDsPattern(long pattern) {
   currentPattern = pattern;
}

void showLEDsPattern() {
   writeShiftReg(currentPattern);
}

void blinkLEDs() {
   if (millis() - checkPointBlink > blinkTime) {
      checkPointBlink = millis();
      blinkON = !blinkON;
   }
   if (blinkON) {
         currentPattern = (1 << (regCount*regWidth)) - 1;
      } else {
         currentPattern = 0;
      }
}

void runLEDs() {
   if (millis() - checkPointRun > runUpdateTime) {
      checkPointRun = millis();
      currentRunLED ++;
      currentRunLED %= regCount * regWidth;
   }
   currentPattern = ~(0b11 << currentRunLED);
}

void runLEDsReverse() {
   if (millis() - checkPointRun > runUpdateTime) {
      checkPointRun = millis();
      currentRunLED = currentRunLED == 0 ? regCount * regWidth - 1: currentRunLED - 1;
   }
   currentPattern = ~(0b11 << currentRunLED);
}