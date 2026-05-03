#include <SPI.h>
#include <MFRC522.h>
#include <HardwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// ---- Variables globales ----
// RFID
  // En caso de quere cambiar de pines, el numero representa el GPIO no el numero Dn de la placa.
#define RST_PIN 22 // D22
#define SS_PIN 5  // D5
MFRC522 rfid(SS_PIN, RST_PIN);

// DFPlayer
HardwareSerial dfSerial(2); // UART2
DFRobotDFPlayerMini dfPlayer;
unsigned long ultimoReplay = 0;
int trackActual = -1;

// Manejo de TAG presente
String uidActual = "";
bool tarjetaPresente = false;

// Manejo de TAG retirada
unsigned long ultimoPing = 0;
unsigned long ultimaRespuesta = 0;

const unsigned long intervaloPing = 100;
const unsigned long timeoutRFID = 500;

// Modo ADMIN
bool modoAdmin = false;
String adminUID = "B4CEEF06";
byte adminPage = 5;

// Manejo de RGB (Catodo comun)
#define LED_R 25
#define LED_G 26
#define LED_B 27

// ---- Prototipos de funciones ----
void detectarNuevaTarjeta();
void verificarPresenciaRFID();
String obtenerUID();
void reproducirCancion(String uidActual);
void breathingEffect();
void apagarLED();
void verificarTerminoCancion();
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
  dfSerial.begin(9600, SERIAL_8N1, 16, 17); // RX2, TX2
  delay(3000);

  if (dfPlayer.begin(dfSerial)) {
    Serial.println("DFPlayer OK");
  } else {
    Serial.println("Error DFPlayer");
  }

  // RGB
  ledcSetup(0, 5000, 8);
  ledcAttachPin(LED_R, 0);

  ledcSetup(1, 5000, 8);
  ledcAttachPin(LED_G, 1);

  ledcSetup(2, 5000, 8);
  ledcAttachPin(LED_B, 2);
}

void loop() {
  if (modoAdmin) {
    manejarModoAdmin();
    return;
  }

  detectarNuevaTarjeta();

  if (tarjetaPresente){
    verificarPresenciaRFID();

    // Detectamos si la canción termino para repetirla
    verificarTerminoCancion();

    // * Elimina esta linea y el else si no quieres agregar el led rgb.
    breathingEffect();
  } else {
    apagarLED();
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
    // Detener la canción actual
    dfPlayer.stop();

    tarjetaPresente = false;
    uidActual = "";
    trackActual = -1;
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
//   Verificamos si ya había una canción guardada o hay que leer una nueva
  if (trackActual == -1) {
    trackActual = leerTrackDesdeTarjeta();
  }

  if (trackActual != -1) {
    Serial.print("Reproduciendo track: ");
    Serial.println(trackActual);

    dfPlayer.volume(15);
    dfPlayer.play(trackActual);
    return;
  }

  Serial.println("No se encontró track en la tarjeta");
}

void breathingEffect() {

  static int brillo = 0;
  static int direccionBrillo = 5;

  static int colorIndex = 0;

  brillo += direccionBrillo;

  if (brillo >= 255 || brillo <= 0) {
    direccionBrillo = -direccionBrillo;

    // Cuando completa una respiración,
    // cambia al siguiente color
    if (brillo <= 0) {
      colorIndex++;
      if (colorIndex > 5) {
        colorIndex = 0;
      }
    }
  }

  int r = 0;
  int g = 0;
  int b = 0;

  switch (colorIndex) {

    case 0: // rojo
      r = brillo;
      break;

    case 1: // morado
      r = brillo;
      b = brillo;
      break;

    case 2: // azul
      b = brillo;
      break;

    case 3: // cyan
      g = brillo;
      b = brillo;
      break;

    case 4: // verde
      g = brillo;
      break;

    case 5: // amarillo
      r = brillo;
      g = brillo;
      break;
  }

  ledcWrite(0, r);
  ledcWrite(1, g);
  ledcWrite(2, b);

  delay(25);
}

void apagarLED() {
  ledcWrite(0, 0);
  ledcWrite(1, 0);
  ledcWrite(2, 0);
}

void verificarTerminoCancion() {
  if (!dfPlayer.available()) return;

  uint8_t type = dfPlayer.readType();
  uint16_t value = dfPlayer.read();

  if (type != DFPlayerPlayFinished) return;
  // Si la canción ya termino, como la tarjeta sigue presente repetimos la canción

  Serial.print("Canción terminada: ");
  Serial.println(value);

  // Definimos tiempo de debounce para evitar multiples reinicios
  if (millis() - ultimoReplay < 200) return; 
  
  delay(100);

  reproducirCancion(uidActual);

  ultimoReplay = millis();
}

// * ==== FUNCIONES MODO ADMIN ====
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