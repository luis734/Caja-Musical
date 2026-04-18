#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 5 // D1
#define SS_PIN 4  // D2

MFRC522 rfid(SS_PIN, RST_PIN);

// ---- UIDs definidos ----
struct RFIDTag {
  byte uid[7];
  byte length;
  const char* mensaje;
};
bool tarjetaPresente = false;
byte uidLlavero[] = {0x7E, 0x68, 0x65, 0x06};
byte uidTarjeta1[]  = {0xB4, 0xCE, 0xEF, 0x06};

RFIDTag tags[] = {
  {{0x53, 0x80, 0x7D, 0x12, 0x13, 0x00, 0x01}, 7, "Sticker 1"},
  {{0x53, 0x7F, 0x7D, 0x12, 0x13, 0x00, 0x01}, 7, "Sticker 2"},
  {{0x53, 0x7E, 0x7D, 0x12, 0x13, 0x00, 0x01}, 7, "Sticker 3"},
};

// ---- Función para comparar ----
const char* buscarTag(byte *uidLeido, byte longitud) {
  int numTags = sizeof(tags) / sizeof(tags[0]);

  for (int i = 0; i < numTags; i++) {

    if (tags[i].length != longitud) continue;

    bool match = true;

    for (byte j = 0; j < longitud; j++) {
      if (uidLeido[j] != tags[i].uid[j]) {
        match = false;
        break;
      }
    }

    if (match) {
      return tags[i].mensaje;
    }
  }

  return NULL; // no encontrado
}

void setup() {
  Serial.begin(115200);
  SPI.begin();
  rfid.PCD_Init();
}

void loop() {

  // Si NO hay tarjeta
  if (!rfid.PICC_IsNewCardPresent()) {
    tarjetaPresente = false; // se liberó
    return;
  }

  // Si ya la procesamos, ignorar
  if (tarjetaPresente) return;

  // Leer tarjeta
  if (!rfid.PICC_ReadCardSerial()) return;

  tarjetaPresente = true;

  // Buscar en tabla
  const char* resultado = buscarTag(rfid.uid.uidByte, rfid.uid.size);

  if (resultado != NULL) {
    Serial.print("Detectado: ");
    Serial.println(resultado);
  } else {
    Serial.println("UID no reconocido");
  }

  rfid.PICC_HaltA();
}