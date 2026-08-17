# LoopDesk - Mobius Pedalboard (Chewie Monsta Clone)

Este proyecto es una pedalera MIDI personalizada diseñada para replicar el flujo de trabajo de "Live Looping" tipo **Ed Sheeran** utilizando el software **Mobius** en Windows. 

La pedalera está construida sobre un **Arduino Pro Micro** y cuenta con retroalimentación visual completa mediante pantallas OLED y LEDs direccionables, funcionando de manera bidireccional con Mobius.

## 🛠️ Características Principales
p
- **Control Total:** 10 switches de pie para funciones de grabación, play, overdub, mute, undo/redo, clear y tap tempo.
- **Retroalimentación Visual (Feedback):**
    - **Pantallas OLED (2x):** Visualización del estado de las pistas y métricas (BPM) en tiempo real.
    - **LEDs WS2812B:** Indicadores de estado por color (Rojo=Rec, Verde=Play, Naranja=Overdub, Cian=Mute).
- **Comunicación Bidireccional:** Arduino envía comandos a Mobius y recibe estados de vuelta para actualizar las luces y pantallas automáticamente.
- **Diseño Robusto:** Optimizado para escenarios con blindaje contra interferencias y diseño ergonómico.

## 📋 Lista de materiales (BOM)

- 1x Arduino Pro Micro (Atmega32U4, 5V / 16MHz)
- 10x Pulsadores de pie momentáneos (Heavy Duty SPST)
- 10x LEDs direccionables WS2812B
- 2x Pantallas OLED SSD1306 (0.96", I2C)
- 1x Módulo Step-Down (LM2596 o MP1584) para alimentación de LEDs
- Hub USB interno y cableado blindado

## 🗺️ Esquema de conexiones

- **Bus I2C:** Ambas pantallas OLED conectadas en paralelo (SDA Pin 2, SCL Pin 3). Requiere modificar la dirección I2C (0x3C y 0x3D).
- **LEDs:** Conectados en cascada al Pin 4.
- **Switches:** Conectados directamente a los pines 5, 6, 7, 8, 9, 10, 14, 15, 16 y 18 (A0) usando `INPUT_PULLUP`.

## 🖥️ Configuración de Mobius

1. Configura el **MIDI Out** en `Configuration > MIDI Devices` hacia tu dispositivo Arduino.
2. Utiliza los scripts de Mobius (.mos) incluidos en este repo para automatizar la retroalimentación visual.
3. Asigna los CC (Control Change) en `Options > MIDI Control` según el mapeo definido en el código.

## 🚀 Instalación

1. Instala las siguientes librerías en tu Arduino IDE:
   - `MIDI Library` (por Francois Best)
   - `Adafruit NeoPixel`
   - `Adafruit SSD1306`
   - `Adafruit GFX Library`
2. Carga el archivo `LooperMIDI.ino` en tu Arduino Pro Micro.
3. Asegúrate de configurar los puentes I2C en tus pantallas OLED para direcciones distintas.

## ⚡ Diseño Eléctrico

**Atención:** No alimentes los 10 LEDs directamente desde el pin 5V del Arduino. Utiliza una fuente de alimentación externa (9V estándar de pedales de guitarra) con un módulo regulador a 5V para garantizar estabilidad y evitar daños al puerto USB del ordenador. Asegura una **tierra común (GND)** entre la fuente externa y el Arduino.

## 📜 Licencia

Este proyecto es de código abierto. Siéntete libre de modificarlo, mejorarlo y compartirlo.

