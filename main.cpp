#include "Arduino.h"
#include "MIDIUSB.h"
#include "Timer.h"
#include "DriverMatrix.h"
#include "DriverLEDs.h"
#include "DrumKit.h"
#include "DriverVolume.h"
#include "DriverLCD.h"

// ============================================================================

#define VELOCITY  127   // velocidad/volumen de notas por defecto

typedef struct _instrument {
   bool mute;           // mute on/off
   int kitSound;       // instrumento del kit correspondiente
   uint8_t pattern;     // patron de sonido (usar uint16_t para 16 botones)
} instrument;

enum operationModes {
   live,             // reproduce el patron de cada instrumento los botones generan sonido en cuanto se presionan (en vivo)
   patternEditor,    // editor de patrones de cada instrumento
   instrumentEditor, // editor de set de instrumentos
   mute              // permite mutear temporalmente cada instrumento
};

const int matrixSize = 8;    // numero de botones en la matriz (actualizar driver ante cambio)

instrument myInstrumentSet[matrixSize];  
int defaultInstrumentSet[] = {
   BASS_DRUM_2,
   SNARE_DRUM_1,
   SIDE_STICK,
   CLOSED_HI_HAT,
   CHINESE_CYMBAL,
   HIGH_AGOGO,
   SHORT_GUIRO,
   CLAVES };

int midiChannel = 10;   // canal por defecto (es el que se suele usar p/percusion)
int bpm = 200;          // tempo por defecto
int bpmStep = 10;       // tamaño del incremento/decremento de BPMs

int currentStep = -1;         // paso actual (va de 0 a matrixSize - 1)
long currentLEDPattern = 0;   // estado actual de los LEDs de la matriz
bool paused = false;

enum operationModes currentMode = live;

rgb_lcd LCD;
bool printLCD = true;

int previousVolume = 127;
int currentVolume = 127;

// auxiliares varios para LCD
bool modeChanged = true;
bool bpmChanged = true;
bool pausePlayChanged = true;

// auxiliares varios para editor de patrones
int instrumentChosen = -1;

// auxiliares para manejo de LEDs
long LEDsPatternAux;

// Setup & Loop ===============================================================

void setup() {
   
   buttonsInit('B', PB4, PB5, PB6);
   timerInit(stepActions);
   LEDsInit();
   volumeInit(0,0,127);
   setTimerBPM(bpm);
   LCD.begin(16, 2);

   setDefaultIntrumentSet();
   setButtonsAction();

   // p/debugging (patrones sencillos) █ ░ 
   myInstrumentSet[0].pattern = 0b00010101;
   // patron de LEDs por defecto
   setLEDsPattern(0b10101010);

   // p/debugging
   // Serial.begin(9600);  
   // me aseguro de que el puerto este abierto antes de empezar a tirarle que imprima
   // while (!Serial);
   // [!] OJO: hace que no sea plug and play, si no esta el monitor del puerto serie
   //          abierto no va a arrancar
   // Serial.println("Status                           ║ Actions");
   // Serial.println("═════════════════════════════════╩════════════════════════════════════════════");
}

void loop() {
   timerScan();    // ejecuta stepActions (cuando corresponda)
   buttonsScan();  // ejecuta funcion de botones de la matriz (cuando corresponda)
   volumeScan();
   updateLCD();
   updateLEDs();
   updateVolume();
}

// ============================================================================

// configura accion correspondiente a cada boton en la matriz 
void setButtonsAction() {

   // Botones de Matriz

   // Boton 0
   buttonPressCallback(matrixButtonPress, BUTTON_0);
   buttonReleaseCallback(matrixButtonRelease, BUTTON_0);
   // Boton 1
   buttonPressCallback(matrixButtonPress, BUTTON_1);
   buttonReleaseCallback(matrixButtonRelease, BUTTON_1);
   // Boton 2
   buttonPressCallback(matrixButtonPress, BUTTON_2);
   buttonReleaseCallback(matrixButtonRelease, BUTTON_2);
   // Boton 3
   buttonPressCallback(matrixButtonPress, BUTTON_3);
   buttonReleaseCallback(matrixButtonRelease, BUTTON_3);
   // Boton 4
   buttonPressCallback(matrixButtonPress, BUTTON_4);
   buttonReleaseCallback(matrixButtonRelease, BUTTON_4);
   // Boton 5
   buttonPressCallback(matrixButtonPress, BUTTON_5);
   buttonReleaseCallback(matrixButtonRelease, BUTTON_5);
   // Boton 6
   buttonPressCallback(matrixButtonPress, BUTTON_6);
   buttonReleaseCallback(matrixButtonRelease, BUTTON_6);
   // Boton 7
   buttonPressCallback(matrixButtonPress, BUTTON_7);
   buttonReleaseCallback(matrixButtonRelease, BUTTON_7);

   // Botones de Control

   // Boton 8
   buttonPressCallback(increaseBPM, BUTTON_8);
   // Boton 9
   buttonPressCallback(decreaseBPM, BUTTON_9);
   // Boton 10
   buttonPressCallback(previousInstrument, BUTTON_10);
   // Boton 11
   buttonPressCallback(nextInstrument, BUTTON_11);
   // Boton 12
   buttonPressCallback(previousMode, BUTTON_12);
   // Boton 13
   buttonPressCallback(nextMode, BUTTON_13);
   // Boton 14
   buttonPressCallback(done, BUTTON_14);
   // Boton 15
   buttonPressCallback(playPause, BUTTON_15);
}

// configura un set de intrumentos por defecto
void setDefaultIntrumentSet() {
   for (int i = 0; i < matrixSize; i++) {
      myInstrumentSet[i].mute = false;
      myInstrumentSet[i].kitSound = defaultInstrumentSet[i];
      myInstrumentSet[i].pattern = 0;
   }
}

// ejecuta acciones que se dan con cada step: reproducir sonidos, led del tempo, etc
void stepActions() {
   // si estoy en pausa me quedo en el paso en el que estoy 
   if (!paused) {
      currentStep = (currentStep + 1) % matrixSize;
      switch (currentMode) {
         case live:
            playPatterns();
            break;
         case mute:
            playPatterns();
            break;
         case patternEditor:
            playPatterns();
            break;
         case instrumentEditor:
            playPatterns();
            break;
         default:
            // switchDefault("stepActions");
            break;
      }
   }
}

// reproduce todos los sonidos habilitados para el step actual
void playPatterns() {
   // miro el patron de cada instrumento y chequeo si corresponde tocar su nota
   for (int i = 0; i < matrixSize; i++) {
      if (!myInstrumentSet[i].mute) {
         if (bitRead(myInstrumentSet[i].pattern, (matrixSize - 1) - currentStep)) { // lee el patron de MSB a LSB
            noteOff(midiChannel, getInstrumentNote(myInstrumentSet[i].kitSound), VELOCITY);
            noteOn(midiChannel, getInstrumentNote(myInstrumentSet[i].kitSound), VELOCITY);
            MidiUSB.flush();
         }
      }
   }
}

// actualiza el texto y color del LCD segun el modo en el que este
void updateLCD() {
   if (modeChanged) {
      switch (currentMode) {
         case live:
            LCD.setRGB(100,204,204); //turquesa o algo asi
            printLinesLCD("      LIVE      "," play something ");
            // p/debugging
            // Serial.println("LCD: |      LIVE      |");
            // Serial.println("     | play something |");
            break;
         case patternEditor:
            if (instrumentChosen < 0) {
               LCD.setRGB(220,100,50);
               // todavia no elegi instrumento
               printLinesLCD(" PATTERN EDITOR ","  select inst.  ");
               // p/debugging
               // Serial.println("LCD: | PATTERN EDITOR |");
               // Serial.println("     |  select inst   |");
            } else {
               LCD.setRGB(220,100,50);
               // ya elegi instrumento, estoy editando su patron
               printLinesLCD(" PATTERN EDITOR ","  editing inst. ");
               // p/debugging
               // Serial.println("LCD: | PATTERN EDITOR |");
               // Serial.print  ("     | editing inst ");
               // Serial.print  (instrumentChosen);
               // Serial.println(" |");
               // [!] ojo con que sea de dos digitos el numero de instrumento
            }
            break;
         case instrumentEditor:
            if (instrumentChosen < 0) {
               LCD.setRGB(129,218,70); //verdecito o algo asi
               // todavia no elegi instrumento
               printLinesLCD("   SET EDITOR   ","  select inst.  ");
               // p/debugging
               // Serial.println("LCD: |   SET EDITOR   |");
               // Serial.println("     |  select inst   |");
            } else {
               LCD.setRGB(129,218,70); //verdecito o algo asi
               // ya elegi instrumento, estoy editando su patron
               printLinesLCD("   SET EDITOR   ","  editing inst. ");
               // p/debugging
               // Serial.println("LCD: |   SET EDITOR   |");
               // Serial.print  ("     | editing inst ");
               // Serial.print  (instrumentChosen);
               // Serial.println(" |");
               // [!] ojo con que sea de dos digitos el numero de instrumento
            }
            break;
         case mute:
            LCD.setRGB(225,219,59); //turquesa o algo asi
            printLinesLCD("      MUTE      ","  select inst.  ");
            // p/debugging
            // Serial.println("LCD: |      MUTE      |");
            // Serial.println("     |  select inst   |");
            break;
         default:
            // switchDefault("updateLCD");
            break;
      }
      modeChanged = false;
   }
   // [!] sacar de aca, a metodos aparte "update BPM" etc  <<<<<<<<<<<<<<<
   if (bpmChanged) {
      // p/debuggin
      // Serial.print("BPM: ");
      // Serial.println(bpm);
      bpmChanged = false;
   }
   if (pausePlayChanged) {
      // p/debugging
      // Serial.println(paused ? "Paused  ◦" : "Playing •");
      pausePlayChanged = false;
   }
}

void updateLEDs() {
   // inicializo variable auxiliar
   LEDsPatternAux = 0;
   // actualizo LEDs segun modo
   switch (currentMode) {
      case live:
         setLEDsPattern((1 << currentStep) | getMatrixButtonsState());
         break;
      case patternEditor:
         if (instrumentChosen < 0) {
            // todavia no elegi instrumento
            runLEDs();
         } else {
            // ya elegi instrumento, estoy editando su patron
            for (int i = 0; i < matrixSize; i++) {
               if (bitRead(myInstrumentSet[instrumentChosen].pattern, (matrixSize - 1) - i)) {
                  LEDsPatternAux |= (1 << i);
               }
            }
            setLEDsPattern(LEDsPatternAux | (1 << currentStep));
         }
         break;
      case instrumentEditor:
         if (instrumentChosen < 0) {
            // todavia no elegi instrumento
            runLEDsReverse();
         } else {
            // ya elegi instrumento, estoy editando su patron
            setLEDsPattern((1 << instrumentChosen));
         }
         break;
      case mute:
         for (int i = 0; i < matrixSize; i++) {
            // activo los LEDs a cada paso segun le corresponde sonar o este muteado
            if (myInstrumentSet[i].mute || bitRead(myInstrumentSet[i].pattern, (matrixSize - 1) - currentStep)) {
               LEDsPatternAux |= (1 << i);
            }
         }
         setLEDsPattern(LEDsPatternAux);
         break;
      default:
         // p/debugging
         // switchDefault("updateLEDs");
         blinkLEDs();
         break;
   }
   showLEDsPattern();
}

void updateVolume() {
   previousVolume = currentVolume;
   currentVolume = getVolume();
   if (previousVolume != currentVolume) {
      volumeChange(midiChannel,currentVolume);
   }
}

void printLinesLCD(char* line1, char* line2) {
   LCD.setCursor(0,0);
   LCD.print(line1);
   LCD.setCursor(0,1);
   LCD.print(line2);
}

// MIDI =======================================================================

void noteOn(byte channel, byte note, byte velocity) {
   midiEventPacket_t noteOn = {0x09, 0x90 | channel, note, velocity};
   MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte note, byte velocity) {
   midiEventPacket_t noteOff = {0x08, 0x80 | channel, note, velocity};
   MidiUSB.sendMIDI(noteOff);
}

void volumeChange(byte channel, byte value) {
   midiEventPacket_t ctrlChange = {0x0B, 0xB0 | channel, 7, value};
   MidiUSB.sendMIDI(ctrlChange);
}

// Acciones de los botones de la matriz =======================================

void matrixButtonPress(int n) {
   // p/debugging
   // Serial.print("                                   [Button ");
   // Serial.print(n);
   // Serial.println(": ↓]");

   switch (currentMode) {
      case live:
         // envio mensaje MIDI
         noteOn(midiChannel, getInstrumentNote(myInstrumentSet[n].kitSound), VELOCITY);
         MidiUSB.flush();
         // p/debugging
         // Serial.print("Played: INST_");
         // Serial.println(n);
         break;
      case patternEditor:
         if (instrumentChosen < 0) {
            // todavia no elegi -> marco como elegido al boton presionado
            instrumentChosen = n;
            modeChanged = true;
         } else {
            // ya fue elegido el instrumento -> edito su patron
            // toggle del bit en la posicion n (ojo, de adelante hacia atras, que es como se reproduce el patron)
            // quedaria mal si el boton 0 hiciera toggle al bit 0 del patron
            myInstrumentSet[instrumentChosen].pattern ^= (1 << (matrixSize - 1) - n);
            // p/debugging
            // Serial.print("New pattern: ");
            // for(int i = matrixSize - 1; i >= 0; i--) {
            //    Serial.print(bitRead(myInstrumentSet[instrumentChosen].pattern,i) ? "▇ " : "▁ ");
            // }
            // Serial.println("");
         }
         break;
      case instrumentEditor:
         if (instrumentChosen < 0) {
            // todavia no elegi -> marco como elegido al boton presionado
            instrumentChosen = n;
            modeChanged = true;
         }
         break;
      case mute:
         myInstrumentSet[n].mute = !myInstrumentSet[n].mute;
         // p/debugging
         // Serial.print(myInstrumentSet[n].mute ? "Mute ON:  INST_" : "Mute OFF: INST_");
         // Serial.println(n);
         break;
      default:
         // switchDefault("matrixButtonPress");
         break;
   }
}

void matrixButtonRelease(int n) {
   // p/debugging
   // Serial.print("                                   [Button ");
   // Serial.print(n);
   // Serial.println(": ↑]");

   switch (currentMode) {
      case live:
         noteOff(midiChannel, getInstrumentNote(myInstrumentSet[n].kitSound), VELOCITY);
         MidiUSB.flush();
         // p/debugging
         // Serial.print("Played: INST_");
         // Serial.println(n);
         break;
      case mute:
      case patternEditor:
      case instrumentEditor:
         break;
      default:
         // switchDefault("matrixButtonRelease");
         break;
   }
}

// Acciones de los botones de control =========================================

void nextMode() {
   // habilito cambio de modo siempre que no este en el medio de una edicion
   if (instrumentChosen < 0) {
      switch (currentMode) {
         case live:
            currentMode = patternEditor;
            break;
         case patternEditor:
            currentMode = instrumentEditor;
            break;
         case instrumentEditor:
            currentMode = mute;
            break;
         case mute:
            currentMode = live;
            break;
         default:
            // switchDefault("nextMode");
            break;
      }
      modeChanged = true;
      // p/debugging
      // Serial.println("                                   [Command: Mode >]");
   }
}

void previousMode() {
   // habilito cambio de modo siempre que no este en el medio de una edicion
   if (instrumentChosen < 0) {
      switch (currentMode) {
         case live:
            currentMode = mute;
            break;
         case patternEditor:
            currentMode = live;
            break;
         case instrumentEditor:
            currentMode = patternEditor;
            break;
         case mute:
            currentMode = instrumentEditor;
            break;
         default:
            // switchDefault("previousMode");
            break;
      }
      modeChanged = true;
      // p/debugging
      // Serial.println("                                   [Command: Mode <]");
   }
}

void increaseBPM() {
   bpm = (bpm + bpmStep > 300)? 300 : bpm + bpmStep;
   setTimerBPM(bpm);
   bpmChanged = true;
   // p/debugging
   // Serial.println("                                   [Command: BPM +]");
}

void decreaseBPM() {
   bpm = (bpm - bpmStep < 0 )? 0 : bpm - bpmStep;
   setTimerBPM(bpm);
   bpmChanged = true;
   // p/debugging
   // Serial.println("                                   [Command: BPM -]");
}

void nextInstrument() {
   // si estaba en modo edicion de instrumento y ya habia elegido instrumento -> salgo de la edicion de ese instrumento
   if ((currentMode == instrumentEditor) && (instrumentChosen >= 0)) {   
      myInstrumentSet[instrumentChosen].kitSound = getNextInstrument(myInstrumentSet[instrumentChosen].kitSound);
      // preview de sonido
      noteOff(midiChannel, getInstrumentNote(myInstrumentSet[instrumentChosen].kitSound), VELOCITY);
      noteOn(midiChannel, getInstrumentNote(myInstrumentSet[instrumentChosen].kitSound), VELOCITY);
      MidiUSB.flush();
      // p/debugging
      // Serial.println("                                   [Command: Next Instrument >]");
   }
}

void previousInstrument() {
   // si estaba en modo edicion de instrumento y ya habia elegido instrumento -> salgo de la edicion de ese instrumento
   if ((currentMode == instrumentEditor) && (instrumentChosen >= 0)) {   
      myInstrumentSet[instrumentChosen].kitSound = getPreviousInstrument(myInstrumentSet[instrumentChosen].kitSound);
      // preview de sonido
      noteOff(midiChannel, getInstrumentNote(myInstrumentSet[instrumentChosen].kitSound), VELOCITY);
      noteOn(midiChannel, getInstrumentNote(myInstrumentSet[instrumentChosen].kitSound), VELOCITY);
      MidiUSB.flush();
      // p/debugging
      // Serial.println("                                   [Command: Previous Instrument <]");
   }
}

void playPause() {
   paused = !paused;
   pausePlayChanged = true;
   // p/debugging
   // Serial.println("                                   [Command: Pause/Play]");
}

void done() {
   // si estaba en modo edicion y ya habia elegido instrumento -> salgo de la edicion de ese instrumento
   if ((currentMode == patternEditor || currentMode == instrumentEditor) && (instrumentChosen >= 0)) {
      instrumentChosen = -1;
      // [!] HORRIBLE OTRA VEZ
      modeChanged = true;
      // p/debugging
      // Serial.println("                                   [Command: Editing done]");
   }
}


// PARA DEBUGGING =============================================================

// void switchDefault(char* str) {
//    p/debugging
//    Serial.print("                                   [!: Switch Default - ");
//    Serial.print(str);
//    Serial.println("]");
// }