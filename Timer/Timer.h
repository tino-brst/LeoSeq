// inicializa el timer
void timerInit(void (*_callback)());

// setea base de tiempo
void setTimerBPM(int bpm);

// chequea si hubo interrupcion del timer y ejecuta su funcion de callback
void timerScan();