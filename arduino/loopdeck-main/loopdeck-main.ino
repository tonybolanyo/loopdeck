#include <usbmidi.h>
#include <FastLED.h>

// ---------------- Configuración ----------------

const byte MIDI_CHANNEL = 0;        // 0..15
const int  CC_BASE = 20;            // primer CC = 20 + buttonID

const byte ROWS = 3;
const byte COLS = 4;

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

// ---------------- Configuración LEDs WS2812B ----------------

#define LED_PIN     9
#define NUM_LEDS    10
#define BRIGHTNESS  100

CRGB leds[NUM_LEDS];

// Estados de pista: 0..3 según CC 51..54
// trackState[0] = pista 1, ..., trackState[3] = pista 4
byte trackState[4] = {0, 0, 0, 0};


// ---------------- Helpers MIDI ----------------

// Envía un Control Change (CC)
void sendCC(byte control, byte value, byte channel = MIDI_CHANNEL) {
  // CC: 0xB0 + (channel-1), control, value
  uint8_t status = 0xB0 | ((channel) & 0x0F);
  USBMIDI.write(status);
  USBMIDI.write(control & 0x7F);
  USBMIDI.write(value & 0x7F);
}

// ---------------- Mapeo estado → color ----------------

CRGB colorForTrackState(byte state) {
  switch (state) {
    case 0: // vacía / reset
      return CRGB(0, 0, 0);          // apagado
    case 1: // grabando / record
      return CRGB(255, 40, 40);      // rojo
    case 2: // reproduciendo / play
      return CRGB(40, 255, 40);      // verde
    case 3: // sobrecapa / overdub
      return CRGB(255, 200, 0);      // amarillo
    case 4: // silenciada / mute
      return CRGB(40, 40, 120);      // azul oscuro
    default:
      return CRGB(0, 0, 0);
  }
}

// ---------------- Actualizar LEDs según estado de pistas ----------------
void updateTrackLeds() {
  for (byte i = 0; i < 4; i++) {
    leds[i] = colorForTrackState(trackState[i]);
  }
  FastLED.show();
}

// ---------------- Leer MIDI entrante (CC 51..54) ----------------
void readMidiInput() {
  // USBMIDI se usa como Stream: leemos bytes uno a uno
  while (USBMIDI.available() >= 3) {
    uint8_t status = USBMIDI.read();
    uint8_t data1  = USBMIDI.read();
    uint8_t data2  = USBMIDI.read();

    // Solo nos interesan Control Change (0xB0..0xBF)
    if ((status & 0xF0) == 0xB0) {
      byte channel = (status & 0x0F) + 1; // 1..16
      byte cc      = data1;
      byte value   = data2;

      // Filtramos por CC 51..54
      if (cc >= 51 && cc <= 54) {
        byte trackIndex = cc - 51; // 0..3
        // Opcional: filtrar por canal si Mobius usa uno concreto
        // if (channel != midiChannel) continue;

        // Limitamos valor a 0..4 por seguridad
        if (value <= 4) {
          trackState[trackIndex] = value;
          updateTrackLeds();
        }
      }
    }
  }
}

// ---------------- Setup ----------------

void setup() {
  // LEDs
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();

  // No hace falta USBMIDI.begin(), se usa como Stream.
  // Pero sí hay que llamar a USBMIDI.poll() en loop.

  for (byte r = 0; r < ROWS; r++) {
    pinMode(rowPins[r], INPUT_PULLUP);
  }

  for (byte c = 0; c < COLS; c++) {
    pinMode(colPins[c], OUTPUT);
    digitalWrite(colPins[c], HIGH); // inactivo
  }

  // Inicializa LEDs de pista apagados
  for (byte i = 0; i < 4; i++) {
    trackState[i] = 0;
    leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show();
}


// ---------------- Loop ----------------

void loop() {
  // Leer MIDI entrante (Mobius → Pro Micro)
  readMidiInput();

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
