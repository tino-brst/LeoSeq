#include "Arduino.h"
#include "Tools.h"
#include "DriverSPShiftReg.h"

static int loadPin;     // RCLK (Reg pin 12)
static int clockPin;    // SRCLR Reg pin 11)
static int dataPin;     // SER Reg pin 14)
   
static int pulseWidth;  // en microsegundos
static int regWidth;    // tamaño de registros
static int regCount;    // numero de registros (para uso en cadena)

void spShiftRegInit(int _loadPin, int _clockPin, int _dataPin, int _regCount) {
   
   loadPin = _loadPin;
   clockPin = _clockPin;
   dataPin = _dataPin;
   regCount = _regCount;
   regWidth = 8;
   pulseWidth = 5;

   // configuro controles del SR y su estado inicial
   DDRB |= (1 << loadPin);
   DDRD |= (1 << clockPin);
   PORTB &= ~(1 << loadPin);
   PORTD &= ~(1 << clockPin);

   // configuro salida serie del SR
   DDRC |= (1 << dataPin); 
}

void writeShiftReg(long value) {
   // p/debugging
   // Serial.print("writeShiftReg: ");
   // printByte(value);

   // recorro bits en 'value' de MSB a LSB y envio
   for (int i = regWidth * regCount - 1; i >= 0; i--) {
      // cargo valores recibidos en el registro de desplazamiento interno
      if (value & (1 << i)){
         PORTC |= (1 << dataPin);
      } else {
         PORTC &=  ~(1 << dataPin);
      }
      // cargo la nueva data al registro interno
      PORTD |= (1 << clockPin);
      delayMicroseconds(pulseWidth);
      PORTD &= ~(1 << clockPin);
   }
   // pongo valores de registro interno en las salidas
   PORTB |= (1 << loadPin);
   delayMicroseconds(pulseWidth);
   PORTB &= ~(1 << loadPin);
}

