// Pustaka yang digunakan
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SPI.h>
#include <MFRC522.h>

// Koneksi
#define WIFISSID "YOUR_WIFI_NAME"
#define PASSWORD "YOUR_WIFI_PASSWORD"
#define ACCESSKEY "YOUR_ANTARES_ACCESSKEY"
#define projectName "YOUR_ANTARES_APPLICATIONS_NAME"
#define deviceName "YOUR_ANTARES_DEVICE_NAME"
#define serverName "http://platform.antares.id:8080/~/antares-cse/antares-id/"+String(projectName)+"/"+String(deviceName)
WiFiClient client;
HTTPClient http;
String httpRequestData;
int httpResponseCode;

// Sensor
#define SW420_PIN D8 // Pin Antarmuka Sensor Getaran
constexpr uint8_t RST_PIN = D1; // Pin Antarmuka RST Sensor RFID
constexpr uint8_t SDA_PIN = D2; // Pin Antarmuka SDA Sensor RFID
#define UID_RFID1 "YOUR_USERID_RFID1" // User ID-1
#define UID_RFID2 "YOUR_USERID_RFID2" // User ID-2
MFRC522 rfid(SDA_PIN, RST_PIN); // Konstruktor MFRC522 -> rfid

// Aktuator
#define BUZZER_PIN D3 // Pin Antarmuka Buzzer
#define RSOLENOID_DOORLOCK_PIN D4 // Pin Antarmuka Solenoid Door Lock

// Layar
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

// Array untuk menampung User ID baru
byte UniqueIDentifier[4];

// Variabel Global
int vibration; 
String alarm_keamanan;
String doorstate;
String accessCode;
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

// Method untuk mengatur konektivitas
void ConnectToWiFi() {
  WiFi.mode(WIFI_STA); // Membuat perangkat sebagai station
  WiFi.begin(WIFISSID, PASSWORD); Serial.print("Menyambungkan ke jaringan"); // Memulai jaringan
  while (WiFi.status() != WL_CONNECTED) { // Jika tidak berhasil terhubung ke jaringan maka cetak di serial monitor :
    Serial.print("."); delay(500);
  }
  if (WiFi.status() == WL_CONNECTED) { // Jika berhasil terhubung ke jaringan maka cetak di serial monitor :
    Serial.println("\nTelah terhubung ke "+String(WIFISSID)+"\n\n");
  }
}

// Method untuk inisialisasi sensor RFID
void initSensorRFID(){
  SPI.begin(); // Memulai komunikasi SPI
  rfid.PCD_Init(); // Memulai sensor RFID
}

// Method untuk baca sensor RFID
void bacaSensorRFID(){
  // Cek untuk kartu baru
  if(!rfid.PICC_IsNewCardPresent()) { 
    return;
  }
  
  // Verifikasi apakah UID sudah dibaca
  if(!rfid.PICC_ReadCardSerial()) { 
    return;
  }
  if (rfid.uid.uidByte[0] != UniqueIDentifier[0] || rfid.uid.uidByte[1] != UniqueIDentifier[1] || rfid.uid.uidByte[2] != UniqueIDentifier[2] || rfid.uid.uidByte[3] != UniqueIDentifier[3]) {
    // Store NUID into array
    for (byte i = 0; i < 4; i++) {
      UniqueIDentifier[i] = rfid.uid.uidByte[i];
      accessCode.concat(String(UniqueIDentifier[i] < 0x10 ? " 0" : " "));
      accessCode.concat(String(UniqueIDentifier[i], HEX));
    } accessCode.toUpperCase();
    Serial.print(F("\nUID tag :"));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    if(accessCode.substring(1) == UID_RFID1){
      Serial.print("\nPesan RFID : "); Serial.println("Akses Buka Pintu Green House Berhasil"); doorstate = "Open"; 
      responRFID(); // Respon RFID ditampilkan ke LCD
      digitalWrite(RSOLENOID_DOORLOCK_PIN, LOW); delay(1000); // Solenoid door lock: open
    }
    else if(accessCode.substring(1) == UID_RFID2){
      Serial.print("\nPesan RFID : "); Serial.println("Akses Tutup Pintu Green House Berhasil"); doorstate = "Closed"; 
      responRFID(); // Respon RFID ditampilkan ke LCD
      digitalWrite(RSOLENOID_DOORLOCK_PIN, HIGH); delay(1000); // Solenoid door lock: closed
    }  
    else{
      Serial.print("\nPesan RFID : "); Serial.println("Akses Green House Gagal/UID Belum Terdaftar"); doorstate = "Closed"; 
      responRFID(); // Respon RFID ditampilkan ke LCD
      digitalWrite(RSOLENOID_DOORLOCK_PIN, HIGH); delay(1000); // Solenoid door lock: closed
    }
  }
  else {
    Serial.print("\nPesan RFID : "); Serial.println("Akses Green House Steril\n"); doorstate = "Closed"; 
    responRFID(); // Respon RFID ditampilkan ke LCD
    digitalWrite(RSOLENOID_DOORLOCK_PIN, HIGH); delay(1000); // Solenoid door lock: closed
  }
}

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
  
  // Hentikan PICC
  rfid.PICC_HaltA();
  
  // Hentikan enkripsi pada PCD
  rfid.PCD_StopCrypto1();
}

// Method untuk baca sensor SW-420
void bacaSensorSW420(){
  vibration = digitalRead(SW420_PIN); // Mengukur nilai getaran
  if(vibration == HIGH){ // Jika terdeteksi getaran maka:
    digitalWrite(BUZZER_PIN, HIGH); // Buzzer menyala
    alarm_keamanan = "ON"; Serial.println("Status buzzer = "+String(alarm_keamanan)+" - Danger"); // Status Bahaya
  } else { // Jika tidak ada getaran maka:
    digitalWrite(BUZZER_PIN, LOW); // Buzzer mati
    alarm_keamanan = "OFF"; Serial.println("Status buzzer = "+String(alarm_keamanan)+" - Safe"); // Status Aman
  }
}

// Method untuk mengatur tampilan LCD
void LCDinit(){
  lcd.init(); // Memulai LCD
  lcd.backlight(); delay(250); lcd.noBacklight(); delay(250); lcd.backlight(); // Splash Screen
  lcd.setCursor(0,0); lcd.print("Smart GreenHouse"); lcd.setCursor(4,1); lcd.print("Device-2");delay(10000); // Menampilkan data pada LCD
  lcd.clear(); // Menghapus tampilan data yang ada pada LCD
}
void responRFID(){
  lcd.backlight(); lcd.setCursor(0,0); lcd.print("INFO -----------"); lcd.setCursor(0,1); lcd.print("Stt.Pintu:"+String(doorstate)); // Cetak respon RFID pada LCD
  Serial.println("Status Pintu: "+doorstate); // Cetak respon RFID pada serial monitor
}

// Method untuk mengolah nilai sensor dan mengatur otomatisasi aktuator
void Olah_Data(){
  bacaSensorRFID(); // Memanggil method bacaSensorRFID
  bacaSensorSW420(); // Memanggil method bacaSensorSW420
}

// Kirim Data ke Antares
void kirimDataAntares(){
  Olah_Data(); // Memanggil method Olah_Data
  if ((millis() - lastTime) > timerDelay) { // Jika waktu sekarang dikurangi waktu terakhir lebih besar dari 5 detik maka :
    if (WiFi.status() == WL_CONNECTED) { // Jika tersambung ke jaringan maka :
      // Memulai request http
      http.begin(client, serverName);

      // Header http
      http.addHeader("X-M2M-Origin",ACCESSKEY);
      http.addHeader("Content-Type","application/json;ty=4");
      http.addHeader("Accept","application/json");

      // Data sensor semuanya dikirim ke server melalui protokol http
      httpRequestData += "{\"m2m:cin\": { \"con\":\"{\\\"Alarm Keamanan\\\":\\\"";
      httpRequestData += String(alarm_keamanan);
      httpRequestData += "\\\",\\\"Status Pintu\\\":\\\"";
      httpRequestData += String(doorstate);
      httpRequestData += "\\\"}\"}}";
      // Serial.println(httpRequestData); Serial.println(); // Buka komen ini untuk debugging

      // Respon http
      int httpResponseCode = http.POST(httpRequestData);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println("\n");

      // Mengakhiri request http
      http.end();
    }
    else { // Jika tidak tersambung ke jaringan maka :
      Serial.println("WiFi Disconnected");
    } 
    lastTime = millis(); // Untuk menghitung waktu yang telah berlalu sejak pengiriman data terakhir
  }
}

// Method untuk mengatur inisiasi awal
void setup() {
  Serial.begin(115200); // Baudrate untuk papan NodeMCU
  pinMode(SW420_PIN,INPUT); // Inisialisasi pin sw-420 sebagai INPUT
  pinMode(BUZZER_PIN,OUTPUT); // Inisialisasi pin buzzer sebagai OUTPUT
  digitalWrite(BUZZER_PIN, LOW); // Default buzzer: OFF
  pinMode(RSOLENOID_DOORLOCK_PIN,OUTPUT); // Inisialisasi pin solenoid door lock sebagai OUTPUT
  digitalWrite(RSOLENOID_DOORLOCK_PIN, HIGH); // Default solenoid door lock: OFF (Mengunci)
  ConnectToWiFi(); // Memanggil method ConnectToWiFi
  LCDinit(); // Memanggil method LCDinit
  initSensorRFID(); // Memanggil method initSensorRFID
}

// Method ini akan dikerjakan berulang kali
void loop(){
  kirimDataAntares(); // Memanggil method kirimDataAntares
}

// Nama Final Project : Smart Green House (Device-2: NodeMCU)
// Nama Peserta Edspert.id : Devan Cakra Mudra Wijaya, S.Kom.

// **CATATAN: ADA SEDIKIT BUG DI BAGIAN PEMBACAAN RFID** // **MASIH BELUM TERATASI**
