#include "Arduino.h"
#include "DriverPSShiftReg.h"

// loadPin  : LD
// clockPin : CLK
// dataPin  : Qh (serial out)
   
static int pulseWidth = 5; // en microsegundos
static int regWidth = 8;   // tamaño de registros (en este caso el 74LS165, de 8 bits)

static volatile uint8_t* DDRx;
static volatile uint8_t* PORTx;
static volatile uint8_t* PINx;

static selectPort(char port);

void psShiftRegInit(char port, int loadPin, int clockPin, int dataPin) {
   // seleccion de puerto
   selectPort(port);

   // configuro controles del SR y su estado inicial
   *DDRx |= (1 << loadPin);
   *DDRx |= (1 << clockPin);
   *PORTx |= (1 << loadPin);
   *PORTx &= ~(1 << clockPin);
   // configuro entrada serie del SR
   *DDRx &= ~(1 << dataPin); 
}

long readShiftReg(char port, int loadPin, int clockPin, int dataPin, int regCount) {

   // lectura a retornar
   long data = 0;

   // seleccion de puerto
   selectPort(port);

   // cargo A,B,C,...F en registro interno via LD
   *PORTx &= ~(1 << loadPin);
   delayMicroseconds(pulseWidth);
   *PORTx |= (1 << loadPin);

   // obtengo valores de todos los registros en cadena
   for (int i = 0; i < regWidth * regCount; i++) {
      // guardo bit (el primero aparece disponible con la carga)
      data = data << 1;
      data |= bitRead(*PINx,dataPin) ? 1 : 0;
      // dejo listo el proximo
      *PORTx |= (1 << clockPin);
      delayMicroseconds(pulseWidth);
      *PORTx &= ~(1 << clockPin);
   }

   return data;
}

// selecciona puerto a usar
static selectPort(char port) {
   switch(port) {
      case 'B':
         DDRx = &DDRB;
         PORTx = &PORTB;
         PINx = &PINB;
         break;
      case 'C':
         DDRx = &DDRC;
         PORTx = &PORTC;
         PINx = &PINC;
         break;
      case 'D':
         DDRx = &DDRD;
         PORTx = &PORTD;
         PINx = &PIND;
         break;
      case 'E':
         DDRx = &DDRE;
         PORTx = &PORTE;
         PINx = &PINE;
         break;
      default:
         // por defecto usa el B
         DDRx = &DDRB;
         PORTx = &PORTB;
         PINx = &PINB;
         // p/debugging
         Serial.println("psShiftRegister: Choose a port");
         break;
   }
}