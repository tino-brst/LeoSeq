#include "DriverVolume.h"
#include "Arduino.h"

// DEFINICION DE METODOS ---------------------------

static int mapToVolume(int adc_in);

// VARIABLES GLOBALES ------------------------------
static int currentVolume = -1;
static int volumeMin;
static int volumeMax;
static int inputPin;

int adc_in;

// METODOS PUBLICOS --------------------------------

// inicializacion del potenciometro(solicita canal al ADC)
void volumeInit(int _inputPin, int volMin, int volMax) {
	// deshabilito interrupciones globales
	cli();
	
	// solicito canal del ADC y guardo puntero a resultado de las conversiones
	inputPin = _inputPin; 
	
	//Asigno los valores minimo y maximo de volumen
	volumeMin = volMin;
	volumeMax = volMax;
	
	// habilito interrupciones globales
	sei();
}

//	actualiza valor de volumen medido (via ADC)
void volumeScan() { 
	adc_in = analogRead(inputPin);
	// p/debugging
	// Serial.println(adc_in);
	// Serial.print(". ");
	// Serial.print("Volume: ");
	// Serial.println(getVolume());
}

int getVolume() {
	return mapToVolume(adc_in);
}

// METODOS PRIVADOS --------------------------------

// conversion de valor adc (0...1023) a volumen
static int mapToVolume(int adc_in) {
	int volumeValue = map(adc_in,ADC_MIN,ADC_MAX,volumeMax,volumeMin);
	return volumeValue;
}
