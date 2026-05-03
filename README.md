# 🎵 Caja Musical RFID — Tocadiscos Interactivo

Una caja musical tipo tocadiscos construida con **ESP8266 + RFID + DFPlayer Mini**, donde al acercar una tarjeta RFID al lector se reproduce automáticamente una canción almacenada en una MicroSD.

La idea principal es simular la experiencia de colocar un vinilo sobre un tocadiscos:

➡️ Acercas una tarjeta RFID  
➡️ Comienza a reproducirse la música  
➡️ Al retirar la tarjeta, la música se detiene

Actualmente el proyecto funciona sobre protoboard y está pensado como un proyecto personal realizado por gusto, con posibilidad de seguir creciendo hacia una versión física más elaborada con disco giratorio y mejor acabado visual.

---

# ✨ Características

- Reproducción automática de música mediante RFID
- Modo normal de uso (funcionamiento principal)
- Modo administrador para programar tarjetas
- Soporte para tarjetas **MIFARE Ultralight**
- Uso de **DFPlayer Mini + bocina 3W**
- LED RGB con efecto de respiración (opcional)
- Proyecto modular y fácil de expandir

> El LED RGB es únicamente decorativo y puede eliminarse fácilmente sin afectar el funcionamiento principal.

---

# 🧩 Componentes Utilizados

- ESP8266
- RC522 RFID Reader
- DFPlayer Mini
- Bocina de 3W
- LED RGB (opcional)
- MicroSD
- Tarjetas RFID MIFARE Ultralight
- Protoboard
- Cables Dupont

---

# 🔌 Diagramas de Conexión

Dentro de:

```text
/docs/conexiones