#define BUTTON_0 0
#define BUTTON_1 1
#define BUTTON_2 2
#define BUTTON_3 3
#define BUTTON_4 4
#define BUTTON_5 5
#define BUTTON_6 6
#define BUTTON_7 7
#define BUTTON_8 8
#define BUTTON_9 9
#define BUTTON_10 10
#define BUTTON_11 11
#define BUTTON_12 12
#define BUTTON_13 13
#define BUTTON_14 14
#define BUTTON_15 15

// Mapeo pines de reg. de desplazamiento -> botones
// ╔══════════════════════════════╗
// ║ Inputs  : A B C D E F G H ...║
// ║ BUTTON_ : 0 1 2 3 4 5 6 7 ...║
// ╚══════════════════════════════╝

// inicializa matriz de botones
void matrixInit(char _port, int _loadPin, int _clockPin, int _dataPin);

// escanea entradas paralelas
void matrixScan();

// retorna el estado actual de los botones
long getMatrixButtonsState();

// configura la funcion a ejecutar al presionar un boton
void matrixButtonPressCallback(void (*handler)(int n), int button);

// configura la funcion a ejecutar al presionar un boton
void matrixButtonReleaseCallback(void (*handler)(int n), int button);
