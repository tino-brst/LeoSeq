#include "Arduino.h"
#include "DrumKit.h"

static const uint8_t instrumentsCount = 60;
static const uint8_t instrumentsNotes[] = {
   27,28,29,30,31,32,33,34,35,36,
   37,38,39,40,41,42,43,44,45,46,
   47,48,49,50,51,52,53,54,55,56,
   57,58,59,60,61,62,63,64,65,66,
   67,68,69,70,71,72,73,74,75,76,
   77,78,79,80,81,82,83,84,85,86
};

int getInstrumentNote(int kitNumber) {
   return instrumentsNotes[kitNumber];
}

char* getInstrumentName() {
}

int instrumentsAvailableCount() {
   return instrumentsCount;
}

int getNextInstrument(int kitNumber) {
   return (kitNumber + 1) % instrumentsCount;
}

int getPreviousInstrument(int kitNumber) {
   return kitNumber == 0 ? instrumentsCount - 1 : kitNumber - 1;
}

// int randomInstrument()