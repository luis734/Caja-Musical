# 🛠️ Problemas Comunes y Soluciones

Este documento recopila los problemas reales encontrados durante el desarrollo de la **Caja Musical RFID** y cómo fueron solucionados.

La intención es ayudar a otros a evitar errores comunes y acelerar el proceso de implementación.

---

# 📌 Cómo usar este documento

Cada problema incluye:

- **Síntomas** → Qué ocurre
- **Causa** → Por qué ocurre
- **Solución** → Cómo corregirlo

---

# 🔌 Problemas de Hardware

## ❌ El ESP se reinicia constantemente

**Síntomas:**

- Envia un log indicando reinicio forzado
- Imprime símbolos raros de forma constante

**Causa:**

- Se usaban pines incorrectos para comunicación (RX/TX mal conectados)
- Interferencia con el Serial de la placa y el ESP entraba en reset loop/watchdog reset

**Solución:**

- Cambia el monitor serial a 74880 baudios para ver el mensaje de error
- Revisa que tu placa no requiera esos pines para arranque o que no interfiera con la comunicación serial
- Si usas SoftwareSerial cambialo a otros pines

---

## ❌ DFPlayer no reproduce sonido

**Síntomas:**

- No se escucha nada
- El módulo enciende pero no hay audio

**Causa:**

- Bocina mal conectada
- Alimentación insuficiente

**Solución:**

- Conectar la bocina directamente entre:
  - `SPK_1` ↔ bocina
  - `SPK_2` ↔ bocina
- No conectar a GND
- Verificar que la fuente entregue suficiente corriente

---


## ❌ Audio distorsionado o muy bajo 🔥

**Síntomas:**

- Sonido con ruido
- Volumen bajo

**Causa:**

- Fuente inestable
- Bocina no adecuada
- Canciones con distintos niveles de volumen

**Solución:**

- Usar fuente externa estable
- Verificar impedancia de la bocina (3W compatible)
- Aplica un filtro Loudness Normalization para cambiar el nivel del audio

---

## ❌ RC522 no detecta tarjetas

**Síntomas:**

- No aparece nada en el monitor serial

**Causa:**

- Conexión incorrecta
- Voltaje incorrecto

**Solución:**

- Verificar conexiones SPI
- ⚠️ Asegurar uso de **3.3V (🚧 NO 5V)**

---

## ❌ Reinicios del ESP32 al reproducir 🔥

**Síntomas:**

- El ESP se reinicia al iniciar audio

**Causa:**

- Consumo de corriente elevado (DFPlayer + bocina)
- Error de comunicación

**Solución:**

- Usar fuente externa para DFPlayer
- Compartir GND entre todos los módulos
- Si usas puertos que la placa necesita para inicializar, se reinicia constantemente.
- Verifica que no estes usando el protocolo de comunicación correcto

---

# 📡 Problemas de Comunicación

## ❌ DFPlayer no responde a comandos 🔥

**Síntomas:**

- El código corre pero no reproduce nada

**Causa:**

- Confusión en protocolo de comunicación

**Importante:**

> El DFPlayer **NO usa SPI**, usa **UART (Serial)**

**Solución:**

- Usar `SoftwareSerial` o UART real
- Conectar correctamente:
  - TX ↔ RX
  - RX ↔ TX

---

## ❌ Comunicación inestable con DFPlayer

**Síntomas:**

- A veces funciona, a veces no

**Causa:**

- Ruido eléctrico o mala conexión

**Solución:**

- Revisar cables
- Evitar cables largos
- (Opcional) usar divisor de voltaje en RX

---

# 🧠 Problemas de Lógica / Código

## ❌ La canción se reproduce y se detiene inmediatamente 🔥

**Síntomas:**

- La canción inicia pero se corta al instante

**Causa:**

- El RFID se lee constantemente sin control
- Se dispara múltiples veces la reproducción

**Solución:**

- Detectar estado de presencia de tarjeta
- Evitar reiniciar la canción si ya está activa

---

# 💾 Problemas con la MicroSD

## ❌ No reproduce archivos 🔥

**Síntomas:**

- DFPlayer responde pero no reproduce

**Causa:**

- Nombres incorrectos de archivos

**Solución:**

- Usar formato obligatorio:

```text
001.mp3
002.mp3
003.mp3
```
---

# 📘 Lecciones Aprendidas

- El DFPlayer Mini usa UART, no SPI
- El RFID requiere control de estado, no solo lectura
- La MicroSD es crítica (nombres, formato, orden)
- La alimentación es clave para estabilidad
- Evitar múltiples tarjetas mejora mucho la confiabilidad