# 💿 Funcionamiento: RFID ↔ Canciones

Este documento explica cómo se relacionan las **tarjetas RFID** con las canciones reproducidas en la **MicroSD** mediante el DFPlayer Mini.

La lógica del sistema es simple:

> 📀 Cada tarjeta RFID representa una canción

---

# 🧠 Idea General

1. Una tarjeta RFID guarda un número
2. El ESP8266 lee ese número
3. Envía ese número al DFPlayer Mini
4. El DFPlayer reproduce la canción correspondiente en la MicroSD

---

# 📡 Estructura de las Tarjetas RFID (MIFARE Ultralight)

Las tarjetas utilizadas son del tipo **MIFARE Ultralight**, que tienen una memoria organizada de la siguiente forma:

- 16 páginas (pages)
- Cada página tiene 4 bytes

## 📋 Distribución básica

| Página | Contenido |
|---|---|
| 0 - 3 | Información del fabricante (incluye UID) |
| 4 en adelante | Memoria libre para usuario |

---

## 🔑 UID de la tarjeta

Las primeras páginas contienen el identificador único (UID), que:

- Es de solo lectura
- Identifica cada tarjeta de forma única
- Se usa en el proyecto para definir la **tarjeta admin (master)**

---

# ✍️ Dónde se guarda la canción

El sistema escribe el número de canción en:

### 👉 Página 4 (quinta página)
Esto se hace en el **modo administrador**.

---

# ⚙️ ¿Qué se guarda exactamente?

Se guarda un número en formato texto, por ejemplo:

```text
5
```
ó
```text
12
```

Este número representa la canción que se va a reproducir.

---

# 🔄 Flujo completo

## 🔹Paso 1: Lectura de tarjeta

El RC522 detecta una tarjeta y el ESP32:
- Lee la página 4
- Obtiene el número almacenado

## 🔹Paso 2: Envío al DFPlayer

- El ESP32 envía ese número al DFPlayer Mini meidante
UART (Serial)

## 🔹Paso 3: Reproducción

- El DFPlayer Mini busca en la MicroSD el archivo correspondiente a ese número.

---

# 💾 Formato de las canciones en la MicroSD

Las canciones deben nombrarse con un formato especifico:

## 👉 3 dígitos con ceros a la izquierda

## 📀 Ejemplos
| Número en RFID | Archivo en MicroSD |
|---|---|
| 1 | 001.mp3 |
| 2 | 002.mp3 |
| 3 | 003.mp3 |
| 10 | 010.mp3 |
| 12 | 012.mp3 |