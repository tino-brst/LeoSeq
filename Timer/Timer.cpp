#include "Arduino.h"
#include "Timer.h"

// Variables ==================================================================

static volatile long timer = 0;        // va de 0 a timerTop - 1
static volatile long timerTop = 1000;  // 1 segundo por defecto
static volatile bool done = false;     // habilita ejecucion de funcion de callback

static void (*callback)();             // funcion a ejecutar ante interrupcion de timer

// Metodos publicos ===========================================================

// inicializa el timer
void timerInit(void (*_callback)()) {
   // seteo funcion de callback
   callback = _callback;

   // configuro Timer3 para generar interrupciones cada 1 milisegundo
   // seteo valor de comparacion (ante este valor se resetea el timer)
   OCR3AH = 0x3E;           
   OCR3AL = 0x80;
   // habilito interrupciones por timer
   TIMSK3 |= (1 << OCIE3A);
   // activo el modo CTC (Clear Time on Compare match)
   TCCR3B |= (1 << WGM12);
   // configuro prescaler (sin prescaling)
   TCCR3B |= (1 << CS10);   
}

// setea base de tiempo
void setTimerBPM(int bpm) {
   timerTop = (60.0 / (float)bpm) * 1000.0;
   // 60 / bpm : cada cuanto suena una nota en un minuto
   // ejemplo: con bpm = 60 serian 60 beats por min (60/60 = 1 -> suena una nota por segundo),
   //          entonces deberia tener una interrupcion cada 1 seg (timerTop = 1000)
}

// chequea si hubo interrupcion del timer y ejecuta su funcion de callback
void timerScan() {
   if (done) {
      // ejecuto funcion de callback
      callback();
      done = false;
   }
}

// Interrupciones =============================================================

// atencion a interrupciones del timer
ISR(TIMER3_COMPA_vect) {
   // avanzo el timer
   timer = (timer + 1) % timerTop;
   if (timer == 0) {
      // habilito ejecucion de funcion de callback
      done = true;
   }
}

