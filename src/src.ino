#include <SPI.h>
#include <MFRC522.h>
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// En caso de quere cambiar de pines, el numero representa el GPIO no el numero Dn de la placa.
#define RST_PIN 22 // D22
#define SS_PIN 5  // D5
MFRC522 rfid(SS_PIN, RST_PIN);

// DFPlayer
HardwareSerial dfSerial(2); // UART2
DFRobotDFPlayerMini dfPlayer;

// ---- Variables globales ----
bool tarjetaPresente = false;

unsigned long ultimaLectura = 0;
const unsigned long TIMEOUT = 1500;

byte ultimoUID[10];
byte ultimoSize = 0;

// Estructura de las TAGs
struct RFIDTag {
  byte uid[7];
  byte length;
  int track;
};

// BD
RFIDTag tags[] = {
  {{0x53, 0x80, 0x7D, 0x12, 0x13, 0x00, 0x01}, 7, 1},
  {{0x53, 0x7F, 0x7D, 0x12, 0x13, 0x00, 0x01}, 7, 2},
  {{0x53, 0x7E, 0x7D, 0x12, 0x13, 0x00, 0x01}, 7, 3},
};

// ---- Prototipos de funciones ----
int buscarTrack(byte *uidLeido, byte longitud);
bool hayTarjetaPresente();
void detectarNuevaTarjeta();

void setup() {
  Serial.begin(115200);

  // RFID
  SPI.begin();
  rfid.PCD_Init();

  // DFPlayer
  delay(2000);
  dfSerial.begin(9600, SERIAL_8N1, 16, 17); // RX2, TX2
  delay(1000);

  if (dfPlayer.begin(dfSerial)) {
    Serial.println("DFPlayer OK");
    dfPlayer.volume(20);
  } else {
    Serial.println("Error DFPlayer");
  }
}

void loop() {

  if (!rfid.PICC_IsNewCardPresent()) {
    tarjetaPresente = false;
    return;
  }

  if (tarjetaPresente) return;

  if (!rfid.PICC_ReadCardSerial()) return;

  tarjetaPresente = true;

  int track = buscarTrack(rfid.uid.uidByte, rfid.uid.size);

  if (track != -1) {
    Serial.print("Reproduciendo track: ");
    Serial.println(track);

    dfPlayer.play(track); // 🔥 AQUÍ YA FUNCIONA TODO
  } else {
    Serial.println("UID no reconocido");
  }

  rfid.PICC_HaltA();
}

// ---- Función para comparar tags validas ----
int buscarTrack(byte *uidLeido, byte longitud) {
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
      return tags[i].track;
    }
  }

  return -1; // no encontrado
}