# El código

### Estructura del código

El firmware está optimizado para actuar con total compatibilidad
basándose en la configuración que está definida en el README del proyecto:

- **Gestión de las Pantallas OLED:** Inicializa las dos pantallas por el bus **I2C**,
  asignando la dirección `0x3C` para la pantalla de visualización de pistas
  (Tracks 1 a 4) y `0x3D` para la pantalla global que muestra el **BPM gigante** y
  el modo de ejecución.
- **Control de LEDs direccionables (WS2812B):** Asigna el **Pin 4** para enviar
  datos en cascada. El código lee las respuestas MIDI de Mobius y traduce
  los estados en colores específicos: Apagado (Vacío), **Rojo** (Grabación),
  **Verde** (Reproducción), **Naranja** (Overdub) y **Cian** (Mute).
- **Lectura de los 10 Switches:** Configura los pines físicos utilizando las
  resistencias `INPUT_PULLUP` de Arduino. Integra un algoritmo de debounce
  (antirrebote) por software para evitar falsas pulsaciones dobles al pisar
  los interruptores en el escenario.
- **Comunicación MIDI Bidireccional:** Envía comandos _Control Change_
  (CC del 21 al 30) hacia Mobius al pisar los botones, y se mantiene escuchando
  activamente las respuestas de Mobius (CC del 51 al 54) para actualizar las
  luces y pantallas de manera sincronizada.

## Requisitos de librerías para compilar

Para poder cargar este código en la Arduino Pro Micro desde el entorno de
Arduino IDE, es necesario tener instaladas las siguientes librerías desde
el Gestor de Librerías oficial:

- MIDI Library (por Francois Best)
- Adafruit NeoPixel
- Adafruit SSD1306
- Adafruit GFX Library


## Retroalimentación desde Mobius

En el código de Arduino (usando librerías como MIDI.h o Control_Surface),
es necesario configurar una función de escucha (Callback) para los mensajes
MIDI entrantes desde Mobius.

Cuando Arduino detecte un mensaje en el CC 51 (Pista 1), leerá el valor y
se debe comportar de la siguiente forma:

1. Si el valor es 1, Arduino ejecuta la instrucción para encender el
   LED 1 en Rojo y escribe en la Pantalla OLED 1 el texto "T1: REC".
2. Si el valor es 2, cambia el LED 1 a Verde y actualiza la Pantalla
   OLED 1 a "T1: PLAY".

Este sistema es extremadamente robusto. Incluso si cambia el estado de Mobius usando el ratón o el teclado del PC,
la pedalera física se actualizará instantáneamente de forma _mágica_, calcando el comportamiento de las pedaleras profesionales de escenario.

