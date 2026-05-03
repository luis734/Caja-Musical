# ⚙️ Modo Administrador

Este es un modo especial para progrtamar nuevas tarjetas RFID.

## ¿Cómo entrara?

Hay una tarjeta predefinida como Tarjeta Master, su UID está hardcodeado en el código fuente en la variable global:

```text
adminUID
```

### Al acercar esta tarjeta:
➡️ El sistema entra en Modo Admin
### Al acercarla de nuevo:
⬅️ El sistema sale del Modo Admin

---

# ¿Cómo programar una tarjeta?

Estando en Modo Admin

### Paso 1

Acerca una tarjeta RFID MIFARE Ultralight (y mantenla sobre el lector)

### Paso 2

Desde el Monitor Serial, envía el número de canción desdeado
Ejemplo:

```text
12
```

### Paso 3

Ese valor se escribe en la memoria de la tarjeta RFID, ya puedes retirar la tarjeta del lector

Ahora al acercar esta tarjeta al lector reproducirá:

```text
012.mp3
```

> [!NOTE]
> Las Tags RFID solo reproducen la canción en modo normal.
> En modo admin siempre se intentaran reprogramar.

### Programar múltiples tarjetas

Puedes seguir acercando nuevas tarjetas y asignarles números de canciones una por una.

## 💡 Como hacer tu propia Tarjeta MASTER

Tienes que identificar cual es el UID de la Tarjeta que designaras como MASTER, desde el IDE de Arduino en la librería `MFRC522.h` puedes revisar ejemplos simples para:

- Leer los datos de la tarjeta
- Reprogramar UID de alguna tarjeta
- Entre otros ejemplos muy utiles.

De esta forma puedes ajustarte al UID definida o cambiarla por la que necesites.
