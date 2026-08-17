# Especificar dirección de las pantallas OLED

Para cambiar la dirección I2C en las pantallas OLED de 0.96 pulgadas
y poder usar las dos al mismo tiempo, debemos localizar un pequeño
puente de soldadura en la parte trasera de la placa de circuito de
la pantalla.

Por defecto, casi todas las pantallas OLED chinas vienen configuradas
de fábrica en la dirección `0x3C`. Al modificar una de ellas para que
sea `0x3D`, la placa Arduino Pro Micro podrá enviar textos diferentes
a cada pantalla usando exactamente los mismos dos cables.

## Pasos para realizar la modificación

1. **Localizar el puente "I2C Address":** Dar la vuelta a la pantalla
   OLED. Cerca de los pines superiores o en el centro de la placa,
   hay una etiqueta impresa que dice **IIC Address, ADDR**, o **0x3C / 0x3D**.
   Justo al lado, se pueden ver tres pequeños pads de cobre con una
   resistencia SMD de valor cero (o un puente de soldadura) uniendo
   el pad central con el pad marcado como `0x3C`.
2. **Mover el puente de posición:**
   - Con un desoldador (Opción Limpia): calentar con la punta del
     soldador la pequeña resistencia que está uniendo el pad central
     con `0x3C` y retírala con cuidado usando una pinza o una malla
     desoldadora. Luego, poner una pequeña gota de estaño para unir
     el pad central con el pad marcado como `0x3D`.
   - Con un puente de estaño (Opción Rápida): Si la placa no tiene
     resistencia física sino solo tres pads con una unión de pista
     interna, raspar ligeramente la pista que une el centro con
     `0x3C` con un cúter para cortarla. Luego, aplicar calor y
     añadir una gota de estaño que conecte el pad del centro con
     el pad de 0x3D.
3. **Verificación visual:** Asegúrate de que el pad central solo
   toque el pad de `0x3D`. Si los tres pads se quedan unidos por
   accidente con demasiado estaño, la pantalla provocará un
   cortocircuito en el bus I2C y la placa Arduino se congelará al
   arrancar.

## ⚠️  Consejo crucial de conexión eléctrica

Una vez que soldada la pantalla, ambas se conectan directamente en paralelo:

- VCC con VCC
- GND con GND
- SDA con SDA (Pin 2 del Pro Micro)
- SCL con SCL (Pin 3 del Pro Micro)

Al usar dos pantallas en la misma línea, la resistencia total del bus I2C cambia.
Las pantallas OLED ya incluyen resistencias de pull-up internas en sus líneas
`SDA` y `SCL`. Conectar dos pantallas en paralelo suele funcionar perfectamente
de forma directa con la Arduino Pro Micro. Sin embargo, si la pantalla modificada
muestra caracteres extraños o parpadea, se puede solucionar añadiendo una
resistencia de 4.7k ohmios desde la línea `SDA` a `VCC`, y otra igual desde la
línea `SCL` a `VCC` para estabilizar las señales de datos.

