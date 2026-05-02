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
String uidActual = "";
bool tarjetaPresente = false;

unsigned long ultimoPing = 0;
unsigned long ultimaRespuesta = 0;

const unsigned long intervaloPing = 100;
const unsigned long timeoutRFID = 500;

// Estructura de las TAGs
struct RFIDTag {
  String uid;
  int track;
};

// BD
RFIDTag tags[] = {
  {"53807D12130001", 1},
  {"537F7D12130001", 2},
  {"537E7D12130001", 3},
};

// ---- Prototipos de funciones ----
int buscarTrack(String uidActual);
bool hayTarjetaPresente(); // TODO
void detectarNuevaTarjeta(); // TODO

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
    dfPlayer.volume(15);
  } else {
    Serial.println("Error DFPlayer");
  }
}

void loop() {
  detectarNuevaTarjeta();

  if (tarjetaPresente){
    verificarPresenciaRFID();
  }
}

void detectarNuevaTarjeta() {
  if (!rfid.PICC_IsNewCardPresent()) return;

  if (!rfid.PICC_ReadCardSerial()) return;

  String nuevoUID = obtenerUID();

  if (nuevoUID != uidActual) {
    uidActual = nuevoUID;
    tarjetaPresente = true;

    reproducirCancion(uidActual);
  }

  ultimaRespuesta = millis();

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void verificarPresenciaRFID() {
  if (millis() - ultimoPing < intervaloPing) return;

  ultimoPing = millis();

  byte bufferATQA[2];
  byte bufferSize = sizeof(bufferATQA);

  MFRC522::StatusCode status = rfid.PICC_WakeupA(bufferATQA, &bufferSize);

  if (status == MFRC522::STATUS_OK) {
    ultimaRespuesta = millis();
  }

  if (millis() - ultimaRespuesta > timeoutRFID) {
    dfPlayer.stop();

    tarjetaPresente = false;
    uidActual = "";
  }
}

String obtenerUID() {
  String uid = "";

  for (byte i = 0; i < rfid.uid.size; i++) {
    if (rfid.uid.uidByte[i] < 0x10) {
      uid += "0"; // Si el dato es entre 0 y 9 agregamos un 0 para mantener el formato correcto.
    }

    uid += String(rfid.uid.uidByte[i], HEX);
  }

  uid.toUpperCase();
  return uid;
}

void reproducirCancion(String uidActual) {
  int track = buscarTrack(uidActual);

  if (track != -1) {
    Serial.print("Reproduciendo track: ");
    Serial.println(track);

    dfPlayer.play(track);
  } else {
    Serial.print("UID no reconocido: ");
    Serial.println(uidActual);
  }
}

int buscarTrack(String uidActual) {
  uidActual.toUpperCase();

  int numTags = sizeof(tags) / sizeof(tags[0]);

  for (int i = 0; i < numTags; i++) {
    if (uidActual == tags[i].uid) {
      return tags[i].track;
    }
  }

  return -1; // No encontrado
}