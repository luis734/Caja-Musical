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

# ⏯️ Comportamiento en tiempo real

### Tarjeta presente

▶️ La música se reproduce continuamente

### Tarjeta retirada

⏹️ La música se detiene

---

# 🧩 Componentes Utilizados

- ESP32
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
```

se incluyen:

- Diagramas de conexión individuales
- Diagrama completo del sistema
- Archivos fuente de Fritzing
- Listado de conexiones para una visualización mas sencilla

> [!NOTE]  
> Actualmente solo se realizó el diagrama de conexiones sobre protoboard.  
> Aun no se realizó diseño para PCB o placa de impresión.

---

# 📄 Documentación

Si necesitas saber como funciona algo mas especificamente puede revisar dentro de:

```text
/docs
```

En la raíz de esta carpeta encontrarás, información importante tipo:

- ¿Cómo funciona el Modo Admin? (`modo_admin.md`)
- ¿Cómo vincular las canciónes de la Micro SD y las RFID Tags? (`rfid_canciones.md`)
- ¿Cómo solucionar errores comúnes que te puedas encontrar? (`problemas_comunes.md`)

---

# 📁 Estructura  del Proyecto

```text
├── docs/
|   └── documentación_características.md
│   └── conexiones/
│       ├── diagramas individuales
│       ├── diagrama completo
│       ├── archivos Fritzing
│       └── listado de conexiones
│
├── src/
│   └── código fuente para ESP32
│
└── README.md
```

---

# 📌 Nota Final

Este proyecto nació como algo personal y experimental, pero está pensado para seguir creciendo hasta convertirse en una verdadera caja musical estilo tocadiscos interactivo.

Mas que solo reproducir música, la idea es crear una experiencia física y visual alrededor de ella.