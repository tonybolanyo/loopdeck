#include <usbmidi.h>

// ---------------- Configuración ----------------

const byte MIDI_CHANNEL = 0;        // 0..15
const int  CC_BASE = 20;            // primer CC = 20 + buttonID

const byte ROWS = 3;
const byte COLS = 4;

// Pines para Pro Micro
byte rowPins[ROWS] = {6, 7, 8};     // filas (salidas)
byte colPins[COLS] = {2, 3, 4, 5};  // columnas (entradas con pull-up)


// Mapa de botones: -1 = sin botón en esa posición
// Aquí usamos 10 botones (0..9), dejamos 2 huecos
int8_t buttonMap[ROWS][COLS] = {
  { 0,  1,  2,  3},
  { 4,  5,  6,  7},
  { 8,  9, -1, -1}   // últimos dos sin botón
};

bool lastState[ROWS][COLS];

// ---------------- Helpers MIDI ----------------

// Envía un Control Change (CC)
void sendCC(byte control, byte value, byte channel = MIDI_CHANNEL) {
  // CC: 0xB0 + (channel-1), control, value
  uint8_t status = 0xB0 | ((channel) & 0x0F);
  USBMIDI.write(status);
  USBMIDI.write(control & 0x7F);
  USBMIDI.write(value & 0x7F);
}


// ---------------- Setup ----------------

void setup() {
  // No hace falta USBMIDI.begin(), se usa como Stream.
  // Pero sí hay que llamar a USBMIDI.poll() en loop.

  for (byte r = 0; r < ROWS; r++) {
    pinMode(rowPins[r], INPUT_PULLUP);
  }

  for (byte c = 0; c < COLS; c++) {
    pinMode(colPins[c], OUTPUT);
    digitalWrite(colPins[c], HIGH); // inactivo
  }
}


// ---------------- Loop ----------------

void loop() {
  USBMIDI.poll(); // obligatorio para que funcione USB MIDI

  // Escaneo de matriz
  for (byte c = 0; c < COLS; c++) {
    // Activar esta columna (poner a LOW)
    digitalWrite(colPins[c], LOW);
      
    // Pequeño delay para estabilizar señales (opcional, pero útil)
    // delayMicroseconds(10);

    for (byte r = 0; r < ROWS; r++) {
      bool pressed = (digitalRead(rowPins[r]) == LOW);


      if (pressed && !lastState[r][c]) {
        // Pulsación: envía CC = 127 (o el valor que quieras)
        int8_t btn = buttonMap[r][c];
        if (btn >= 0) {
          int ccNum = CC_BASE + btn;      // 20 + buttonID
          sendCC(ccNum, 127, MIDI_CHANNEL);
        }
      } else if (!pressed && lastState[r][c]) {
        // Liberación: envía CC = 0
        int8_t btn = buttonMap[r][c];
        if (btn >= 0) {
          int ccNum = CC_BASE + btn;
          sendCC(ccNum, 0, MIDI_CHANNEL);
        }
      }

      lastState[r][c] = pressed;
    }

    // Desactivar columna (volver a HIGH)
    digitalWrite(colPins[c], HIGH);
  }

  // Pequeño delay para debounce y no saturar USB
  delay(5);
}
