// Library
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include "FC28.h"

// Koneksi
#define WIFISSID "YOUR_WIFI_NAME"
#define PASSWORD "YOUR_WIFI_PASSWORD"
#define ACCESSKEY "YOUR_ANTARES_ACCESSKEY"
#define projectName "YOUR_ANTARES_APPLICATIONS_NAME"
#define deviceName "YOUR_ANTARES_DEVICE_NAME"
#define serverName "http://platform.antares.id:8080/~/antares-cse/antares-id/"+String(projectName)+"/"+String(deviceName)
WiFiClient client;
HTTPClient http;
unsigned long lastTime = 0;
unsigned long timerDelay = 5000;
String httpRequestData;
int httpResponseCode;

// Sensor
#define PIN_DHT 13
#define DHTTYPE DHT22
DHT dht(PIN_DHT, DHTTYPE);
#define PIN_LDR 35
#define PIN_MOISTURE 36
FC28Sensor fc28;
int moisture = 0;
int ldr = 0; const float GAMMA = 0.7; const float RL10 = 50; float voltage, resistance, lux; int adcValue = 0;
int temp = 0;
int hum = 0;

// Actuator
#define PIN_WATERPUMP 4
bool relayON = LOW; bool relayOFF = HIGH; // Active Low Trigger
String pump;

// Method untuk mengatur konektivitas
void ConnectToWiFi() {
  WiFi.mode(WIFI_STA); // Membuat perangkat sebagai station
  WiFi.begin(WIFISSID, PASSWORD); // Memulai menyambungkan ke jaringan
  Serial.print("Configuring to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\nTelah terhubung ke "+String(WIFISSID)+"\n\n");
}

// Baca Data Sensor
void bacaSensor() {
  adcValue = analogRead(PIN_LDR); // Baca Tegangan Analog Sensor LDR
  voltage = adcValue * 5/4095.0; // ESP bit=12 -> 4095, 5=Tegangan Referensi
  resistance = 2000 * voltage / (1 - voltage / 5); // Rumus Resistansi Cahaya
  ldr = pow(RL10 * 1e3 * pow(10, GAMMA) / resistance, (1 / GAMMA)); // Rumus Intensitas Cahaya
  moisture = fc28.getSoilMoisture(); // moisture
  temp = dht.readTemperature(); // ekstrak data temperature
  hum = dht.readHumidity(); // ekstrak data humidity
  
  // display data ke serial monitor
  Serial.println("=========================================");
  Serial.println("Suhu Udara: "+String(temp)+"°C");
  Serial.println("Kelembaban Udara: "+String(hum)+"%");
  Serial.println("Kelembaban Tanah: "+String(moisture)+"%");
  Serial.println("Intensitas Cahaya: "+String(ldr)+"lux");
}

// Kendali Otomatis Pompa
void Threshold(){
  if (temp >= 0 && temp < 16) {
    if (hum > 90 && hum <=100) {
      if (ldr >= 500) {  
        digitalWrite(PIN_WATERPUMP, relayOFF);
        pump = "OFF";
      }
    }
  }
  if (temp >= 16 && temp <= 34) {
    if (hum >= 30 && hum <= 90) {
      if (ldr >= 200 && ldr < 500) {  
        digitalWrite(PIN_WATERPUMP, relayOFF);
        pump = "OFF";
      }
    }
  }
  if (temp > 34 && temp <= 100) {
    if (hum >= 0 && hum < 30) {
      if (ldr < 200) {
        digitalWrite(PIN_WATERPUMP, relayON);
        pump = "ON";
      }
    }
  } 
  if (moisture >= 14.5 && moisture < 100){
    digitalWrite(PIN_WATERPUMP, relayOFF);
    pump = "OFF";
  }
  if (moisture >= 11.8 && moisture < 14.5) { 
    digitalWrite(PIN_WATERPUMP, relayOFF);
    pump = "OFF";
  }
  if (moisture >= 0 && moisture < 11.8) {
    digitalWrite(PIN_WATERPUMP, relayON);
    pump = "ON";
  }
  Serial.println("Status Pompa: "+String(pump));
  Serial.println("=========================================");
}

// Kirim Data ke Antares
void kirimAntares() {
  if ((millis() - lastTime) > timerDelay) {
    bacaSensor();
    Threshold();
    if (WiFi.status() == WL_CONNECTED) {
      http.begin(client, serverName);

      http.addHeader("X-M2M-Origin",ACCESSKEY);
      http.addHeader("Content-Type","application/json;ty=4");
      http.addHeader("Accept","application/json");

      httpRequestData += "{\"m2m:cin\": { \"con\":\"{\\\"Suhu Udara (°C)\\\":\\\"";
      httpRequestData += String(temp);
      httpRequestData += "\\\",\\\"Kelembapan Udara (%)\\\":\\\"";
      httpRequestData += String(hum);
      httpRequestData += "\\\",\\\"Kelembapan Tanah (%)\\\":\\\"";
      httpRequestData += String(moisture);
      httpRequestData += "\\\",\\\"Intensitas Cahaya (lux)\\\":\\\"";
      httpRequestData += String(ldr);
      httpRequestData += "\\\",\\\"Status Pompa\\\":\\\"";
      httpRequestData += String(pump);
      httpRequestData += "\\\"}\"}}";
      // Serial.println(httpRequestData);
      // Serial.println();
      
      int httpResponseCode = http.POST(httpRequestData);
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.println("\n");

      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    } 
    lastTime = millis();
  }
  delay(5000);
}

// Method untuk mengatur inisiasi awal
void setup() {
  // inisiasi pin sensor
  fc28.initFC28Sensor(9600, PIN_MOISTURE);
  dht.begin();
  pinMode(PIN_LDR, INPUT);
  pinMode(PIN_WATERPUMP, OUTPUT);
  // Memanggil method ConnectToWiFi
  ConnectToWiFi(); 
}

// Method untuk mengatur perulangan
void loop() {
  kirimAntares();
}

// Nama Final Project : Smart Green House (Device-1: ESP32)
// Nama Peserta Edspert.id : Devan Cakra Mudra Wijaya, S.Kom.
