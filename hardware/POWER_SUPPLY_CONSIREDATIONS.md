# Consideraciones sobre la alimentación del circuito

## El Consumo de corriente (el problema)

- Un LED WS2812B al máximo de brillo en color blanco (Rojo + Verde + Azul encendidos)
consume 60 mA.
- 10 LEDs consumirán un máximo de 600 mA.
- El pin de 5V de la Arduino Pro Micro (cuando se alimenta por USB desde el PC) solo
  puede entregar de forma segura entre 300 mA y 400 mA para todo el circuito
  (incluyendo las pantallas OLED y el propio procesador). Como puede verse, los LEDs
  solos superan por mucho el límite.

## El esquema de alimentación seguro (la solución)

Dado que el proyecto prevé usar un PC, la mejor solución es inyectar energía externa
a los LEDs o usar un Hub USB con alimentación propia, separando las líneas de energía
pero manteniendo la tierra unida.

               [ Hub USB Alimentado / Cargador 5V ]
                             |
             +---------------+---------------+

             | (5V Externos)                 | (GND Externa)
             v                               v
       [ Cadena LEDs ]                 [ Cadena LEDs ]
        Línea VCC (5V)                  Línea GND
             ^                               ^

             | (NO conectar a VCC            | (SÍ conectar a GND
             |  del Arduino)                 |  del Arduino)
             |                               |
       [ Arduino Pro Micro ] <---------------+


## Las 3 reglas de oro para el cableado eléctrico

- **GND Compartido (Tierra Común):** El cable de negativo (GND) de la fuente externa
  de 5V o del Hub **DEBE** unirse con el pin GND de la Arduino Pro Micro. Si no se
  unen las tierras, la señal de datos del Pin 4 se volverá errática y los LEDs
  parpadearán con colores locos o no encenderán.
- **Aislamiento del cable VCC (5V):** El cable de positivo (VCC o 5V) que alimenta a
  los 10 LEDs **NUNCA** debe tocar el pin VCC / 5V del Arduino. Los LEDs deben
  recibir sus 5V directamente de la fuente externa. La placa Arduino se alimentará
  exclusivamente a través de su propio cable USB conectado al PC.
- **Resistencia de protección de datos:** Coloca una resistencia de entre 
  330 y 470 ohmios en el cable de señal que va desde el Pin 4 de la Arduino hasta
  el pin DIN del primer LED de la cadena. Esto absorbe los picos de voltaje y
  evita que se queme el primer LED al encender la pedalera.
- **Condensador de estabilización (opcional pero recomendado):** Conectar un
  condensador electrolítico de 1000 µF a 6.3V o 10V directamente entre las
  líneas generales de 5V y GND de los LEDs. Esto actúa como un "tanque de reserva"
  para amortiguar las caídas de voltaje cuando todos los LEDs cambien de color al mismo tiempo.

