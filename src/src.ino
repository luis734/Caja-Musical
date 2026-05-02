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

// Modo ADMIN
bool modoAdmin = false;
String adminUID = "B4CEEF06";
byte adminPage = 5;

// ---- Prototipos de funciones ----
void detectarNuevaTarjeta();
void verificarPresenciaRFID();
String obtenerUID();
void reproducirCancion();
// MODO ADMIN
void manejarModoAdmin();
bool escribirTrackEnTarjeta(int track);
int leerTrackDesdeTarjeta();

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
    dfPlayer.volume(10);
  } else {
    Serial.println("Error DFPlayer");
  }
}

void loop() {
  if (modoAdmin) {
    manejarModoAdmin();
    return;
  }

  detectarNuevaTarjeta();

  if (tarjetaPresente){
    verificarPresenciaRFID();
  }
}

void detectarNuevaTarjeta() {
  if (!rfid.PICC_IsNewCardPresent()) return;

  if (!rfid.PICC_ReadCardSerial()) return;

  String nuevoUID = obtenerUID();

  if (nuevoUID == adminUID) {

    modoAdmin = !modoAdmin;

    if (modoAdmin) {
      Serial.println("=== MODO ADMIN ACTIVADO ===");
      Serial.println("Acerca una tarjeta para programarla");
    } else {
      Serial.println("=== MODO ADMIN DESACTIVADO ===");
    }

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    delay(1000); // evita doble lectura accidental
    return;
  }

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
  int track = leerTrackDesdeTarjeta();

  if (track != -1) {
    Serial.print("Reproduciendo track: ");
    Serial.println(track);

    dfPlayer.play(track);
  } else {
    Serial.println("No se encontró track en la tarjeta");
  }
}

// FUNCIONES MODO ADMIN
void manejarModoAdmin() {

  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  String nuevoUID = obtenerUID();

  // Si vuelves a acercar la tarjeta ADMIN → salir
  if (nuevoUID == adminUID) {
    modoAdmin = false;

    Serial.println("=== MODO ADMIN DESACTIVADO ===");

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    delay(1000);
    return;
  }

  Serial.println("--------------------------------");
  Serial.print("Tarjeta detectada: ");
  Serial.println(nuevoUID);

  Serial.println("Ingresa numero de track por Serial Monitor:");

  // Esperar entrada por serial
  while (!Serial.available()) {
    delay(10);
  }

  int track = Serial.parseInt();

  if (track <= 0) {
    Serial.println("Track invalido");

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();
    return;
  }

  if (escribirTrackEnTarjeta(track)) {
    Serial.print("Track guardado correctamente: ");
    Serial.println(track);
  } else {
    Serial.println("Error escribiendo track");
  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  Serial.println("Acerca otra tarjeta o la ADMIN para salir");
}

bool escribirTrackEnTarjeta(int track) {
  MFRC522::StatusCode status;

  byte dataPage[4] = {0};

  String trackStr = String(track);

  for (int i = 0; i < trackStr.length() && i < 4; i++) {
    dataPage[i] = trackStr[i];
  }

  status = rfid.MIFARE_Ultralight_Write(
    adminPage,
    dataPage,
    4
  );

  if (status != MFRC522::STATUS_OK) {
    Serial.println("Error escribiendo page Ultralighy");
    return false;
  }

  Serial.println("Track escrito correctamente");

  return true;
}

int leerTrackDesdeTarjeta() {
  MFRC522::StatusCode status;

  byte buffer[18];
  byte size = sizeof(buffer);

  status = rfid.MIFARE_Read(
    adminPage,
    buffer,
    &size
  );

  if (status != MFRC522::STATUS_OK) {
    Serial.println("Error leyendo track");
    return -1;
  }

  // Solo usamos los primeros 4 bytes
  String trackStr = "";

  for (int i = 0; i < 4; i++) {
    if (buffer[i] != 0 && isDigit(buffer[i])) {
      trackStr += (char)buffer[i];
    }
  }

  int track = trackStr.toInt();

  Serial.print("Track leido: ");
  Serial.println(track);

  return track;
}