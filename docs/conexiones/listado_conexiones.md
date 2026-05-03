# 🔌 Conexiones del Proyecto

Este documento describe las conexiones físicas entre los módulos utilizados en la Caja Musical RFID.

El sistema principal está compuesto por:

- ESP32
- RC522 RFID Reader
- DFPlayer Mini
- Bocina de 3W
- LED RGB (opcional)

Actualmente todo el sistema está montado sobre **protoboard**, sin PCB personalizada.

---

# 📍 Protocolos de Comunicación Utilizados


| Módulo        | Protocolo                    |
| ------------- | ---------------------------- |
| RC522         | SPI                          |
| DFPlayer Mini | Serial UART (SoftwareSerial) |
| LED RGB       | GPIO PWM                     |
| Bocina        | Salida directa del DFPlayer  |


---

# 🧠 Consideraciones Importantes

## Niveles de Voltaje

El **RC522 trabaja a 3.3V**, por lo que debe conectarse correctamente para evitar daños.

El **DFPlayer Mini suele trabajar a 5V**, aunque puede funcionar con algunas configuraciones a menor voltaje.

## RX del DFPlayer

Se recomienda usar un pequeño divisor de voltaje en el pin **RX del DFPlayer Mini**, especialmente si se conecta desde una salida de 5V.

Esto ayuda a proteger el módulo.

## Alimentación

La bocina de 3W puede demandar más corriente de la esperada.

Si el audio presenta fallos o reinicios, se recomienda usar una fuente externa estable.

---

# 🔗 ESP32 ↔ RC522 (SPI)

> [!WARNING]
> No conectar el RC522 a 5V.


| ESP32   | RC522    |
| ------- | -------- |
| D5      | SDA / SS |
| D18     | SCK      |
| D23     | MOSI     |
| D19     | MISO     |
| D22     | RST      |
| 3.3V    | 3.3V     |
| GND     | GND      |


---

# 🎵 ESP32 ↔ DFPlayer Mini

| ESP32    | DFPlayer Mini |
| -------- | ------------- |
| TX2      | RX            |
| RX2      | TX            |
| VIN      | VCC           |
| GND      | GND           |

> [!NOTE]
> Verifica los pines reales usados en tu código fuente, especialmente si cambias SoftwareSerial.

---

# 🔊 DFPlayer Mini ↔ Bocina 3W


| DFPlayer Mini | Bocina     |
| ------------- | ---------- |
| SPK_1+        | Cable rojo |
| SPK_2-        | Cable negro|

> [!NOTE]
> No conectar la bocina a GND.  
> Debe ir directamente entre SPK_1 y SPK_2.

---

# 🌈 ESP32↔ LED RGB (Opcional)


| ESP32             | LED RGB      |
| ----------------- | ------------ |
| D25               | R            |
| D26               | G            |
| D27               | B            |
| GND               | Cátodo común |

> [!NOTE]
> Debe haber una resistencia de entre 220 o 330 ohms entre el pin del ESP y cada pata del LED para evitar que se queme, para mas información revisa docs/conexiones/LedRGB.
> El LED RGB usa efecto de respiración decorativo y puede eliminarse fácilmente.

---

# 📋 Resumen de Pines Utilizados


| Pin ESP8266 | Uso         |
| ----------- | ----------- |
| D17 (TX2)   | DFPlayer RX |
| D16 (RX2)   | DFPlayer TX |
| D22         | RC522 RST   |
| D18         | RC522 SCK   |
| D19         | RC522 MISO  |
| D23         | RC522 MOSI  |
| D5          | RC522 SDA   |
| GPIO extra  | LED RGB     |


---

# 🔄 Si usas otra placa (Arduino UNO / ESP8266 / etc.)

Debes revisar principalmente:

- Pines SPI
- Pines Serial
- Voltajes compatibles
- Capacidad de corriente
- Compatibilidad con SoftwareSerial

Puede requerir más cuidado con niveles de voltaje y memoria disponible.

---

# 📁 Diagramas Visuales

En esta misma carpeta encontrarás:

- Diagramas individuales
- Diagrama completo del sistema
- Archivos fuente de Fritzing

Estos diagramas sirven como referencia visual complementaria a esta tabla.

---

