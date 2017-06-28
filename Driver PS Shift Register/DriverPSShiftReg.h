// inicializacion del Registro de Desplazamiento (paralelo -> serie)
void psShiftRegInit(char port, int loadPin, int clockPin, int dataPin);

// escanea entrada de registros y retorna su valor
long readShiftReg(char port, int loadPin, int clockPin, int dataPin, int regCount);