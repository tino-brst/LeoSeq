// inicializacion del Registro de Desplazamiento (paralelo -> serie)
void spShiftRegInit(int _loadPin, int _clockPin, int _dataPin, int _regCount);

// escanea entrada de registros y retorna su valor
void writeShiftReg(long value);
