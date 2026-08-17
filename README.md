# LoopDesk - Mobius Pedalboard (Chewie Monsta Clone)

Este proyecto es una pedalera MIDI personalizada diseñada para replicar el flujo de trabajo de "Live Looping" tipo **Ed Sheeran** utilizando el software **Mobius** en Windows. 

La pedalera está construida sobre un **Arduino Pro Micro** y cuenta con retroalimentación visual completa mediante pantallas OLED y LEDs direccionables, funcionando de manera bidireccional con Mobius.

## 🛠️ Características Principales

- **Control Total:** 10 switches de pie para funciones de grabación, play, overdub, mute, undo/redo, clear y tap tempo.
- **Retroalimentación Visual (Feedback):**
    - **Pantallas OLED (2x):** Visualización del estado de las pistas y métricas (BPM) en tiempo real.
    - **LEDs WS2812B:** Indicadores de estado por color (Rojo=Rec, Verde=Play, Naranja=Overdub, Cian=Mute).
- **Comunicación Bidireccional:** Arduino envía comandos a Mobius y recibe estados de vuelta para actualizar las luces y pantallas automáticamente.
- **Diseño Robusto:** Optimizado para escenarios con blindaje contra interferencias y diseño ergonómico.

El objetivo es controlar 4 pistas independientes de Mobius. Para ello los interruptorees de pie realizan las siguientes funciones:

| Footswitch | Función en Mobius              | Comportamiento LED                      | Acción musical                                                 |
| ---------- | ------------------------------ | --------------------------------------- | -------------------------------------------------------------- |
|    FS1     | Track 1: Rec/Play/Overdub      | Rojo (Rec) Verde (Play) Naranja (Ovd)   | Ritmo base (percusión guitarra)                                |
|    FS2     | Track 2: Rec/Play/Overduv      | Rojo (Rec) Verde (Play) Naranja (Ovd)   | Línea de bajo o primer bloque armónico                         |
|    FS3     | Track 3: Rec/Play/Overduv      | Rojo (Rec) Verde (Play) Naranja (Ovd)   | Acordes principales o pad de fondo                             |
|    FS4     | Track 4: Rec/Play/Overduv      | Rojo (Rec) Verde (Play) Naranja (Ovd)   | Armonías vocales o arreglo secundario                          |
|    FS5     | Mute/Unmute track seleccionado | Apagado (Mute) Cian (Activo)            | Silencia instantáneamente la pista seleccionada                | 
|    FS6     | Undo/Redo                      | Parpadeo blanco rápido                  | Borra la última capa grabada                                   |
|    FS7     | Borra todo (Reset)             | Rojo parpadeante prolongado             | Resetea Mobius por completo                                    |
|    FS8     | Selección de pista (ciclo 1-4) | Magenta (indica pista seleccionada)     | Permite moverse entre pistas para aplicar efectos o mutes      |
|    FS9     | Reverse/Speed Toggle           | Azul (efecto activado)                  | Activa efectos globales (reproducir al revés o mitad velocidad |
|    FS10    | Tap Tempo                      | Parpadea en amarillo al ritmo del reloj | Define los BMP del proyecto antes de empezar a tocar           |

Para saber qué está pasando sin tener que mirar al portátil las dos pantallas muestran información básica:

- **Pantalla 1:** Estado de pistas. Mostrará una cuadrícula con el estado de las 4 pistas simultáneamente. Por ejemplo: T1:PLAY T2:REC / T3:EMPTY T4:MUTE. 
- **Pantalla 2:** Métricas. Mostrará el nombre de la canción/preset seleccionado en la parte superior, y el BPM gigante en el centro junto con un contador de compases (1/4, 2/4, etc.) sincronizado con el script de Mobius.

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

