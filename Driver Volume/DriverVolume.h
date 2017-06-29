#define ADC_MIN 0
#define ADC_MAX 1023 

// inicializacion del LDR (solicita canal al ADC)
void volumeInit(int channel , int volMin, int volMax);

// actualiza valor de luminosidad medido (via ADC)
void volumeScan();

int getVolume();