[![Open Source Love](https://badges.frapsoft.com/os/v1/open-source.svg?style=flat)](https://github.com/ellerbrock/open-source-badges/)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg?logo=github&color=%23F7DF1E)](https://opensource.org/licenses/MIT)
![GitHub last commit](https://img.shields.io/github/last-commit/devancakra/Smart-Green-House-Berbasis-IoT-Mobile-Apps?logo=Codeforces&logoColor=white&color=%23F7DF1E)
![Project](https://img.shields.io/badge/Project-Internet%20of%20Things-light.svg?style=flat&logo=arduino&logoColor=white&color=%23F7DF1E)

# Smart-Green-House-Berbasis-IoT-Mobile-Apps
<strong>Tugas Akhir Edspert.id: Intensive Bootcamp Internet of Things Batch-10</strong><br><br>
Green House adalah bangunan untuk budidaya tanaman yang memiliki struktur atap dan dinding tembus cahaya. Para petani terbantu oleh adanya Green House ini, namun tetap saja memerlukan pengondisian tertentu agar dapat mencapai apa yang diinginkan. Hal ini biasanya dilakukan secara tradisional, sehingga dinilai kurang efektif terutama pada jaman yang penuh dengan teknologi serba canggih seperti sekarang ini. Selain itu, rawannya pencurian dan pembobolan bangunan adalah topik yang perlu diperhatikan selain pemeliharaan sayuran. Proyek ini dibuat untuk membantu para petani agar dapat mengelola Green House yang mereka miliki dengan cepat dan tepat baik dalam jarak yang dekat maupun jarak yang jauh. Proyek ini telah dilaksanakan dan memakan waktu kurang lebih 1 bulan. Sistem yang dibuat ini dapat memantau suhu udara, kelembaban udara, kelembaban tanah, intensitas cahaya, status pompa, status pintu, dan alarm keamanan. Selain itu, sistem ini juga telah disediakan fitur kendali pompa dan kunci pintu secara otomatis, sehingga hal ini dapat meningkatkan efisiensi dalam pemeliharaan sayuran dan keamanan yang ada di area Green House. Sistem ini berbasis Internet of Things (IoT) yaitu dengan menggunakan HTTP sebagai protokol komunikasinya. Antarmuka sistem menggunakan aplikasi MIT App Inventor.

<br><br>

## Kebutuhan Proyek
| Bagian | Deskripsi |
| --- | --- |
| Papan Pengembangan | • DOIT ESP32 DEVKIT V1<br>• NodeMCU V3 ESP8266 Lolin |
| Editor Kode | Arduino IDE |
| Driver | • CH340 USB Driver<br>• CP210X USB Driver |
| Platform IoT | Antares |
| Protokol Komunikasi | • Hypertext Transfer Protocol (HTTP)<br>• Universal Asynchronous Receiver-Transmitter (UART)<br>• Serial Peripheral Interface (SPI)<br>• Inter Integrated Circuit (I2C) |
| Arsitektur IoT | 4 Lapisan |
| Dukungan Aplikasi | MIT App Inventor |
| Bahasa Pemrograman | C/C++ |
| Pustaka Arduino | • WiFi (bawaan)<br>• HTTPClient (bawaan)<br>• ESP8266WiFi (bawaan)<br>• ESP8266HTTPClient (bawaan)<br>• Wire (bawaan)<br>• SPI (bawaan)<br>• DHT_sensor_library<br>• LiquidCrystal_I2C<br>• ESP_FC28_Library<br>• MFRC522 |
| Aktuator | • Submersible pump (x1)<br>• Solenoid Door Lock (x1)<br>• Piezo buzzer (x1) |
| Sensor | • FC-28: Kelembaban Tanah Resistif (x1)<br>• LDR: Resistor Cahaya (x1)<br>• DHT22: Suhu & Kelembaban Udara (x1)<br>• SW-420: Getaran (x1)<br>• Pembaca RFID (x1) |
| Layar | LCD I2C (x1) |
| Komponen Lainnya | • Kabel USB Mikro - USB tipe A (x1)<br>• Kabel jumper (1 set)<br>• Switching power supply 12V 3A (x1)<br>• Electrical relay 1 channel (x1)<br>• Step Down LM2596 Adjustable 3A DC-DC (x2)<br>• Breadboard (x2)<br>• Papan ekspansi NodeMCU (x1)<br>• Kotak casing (x1)<br>• Kartu RFID (x2)<br>• Connector male jack DC (x1)<br>• Baut plus (1 set)<br>• Mur (1 set) |

<br><br>

## Unduh & Instal
1. Arduino IDE

   <table><tr><td width="810">

   ```
   https://bit.ly/ArduinoIDE_Installer
   ```

   </td></tr></table><br>

2. CH340 USB Driver

   <table><tr><td width="810">
   
   ```
   https://bit.ly/CH340_USB_Driver
   ```

   </td></tr></table><br>

3. CP210X USB Driver

   <table><tr><td width="810">
   
   ```
   https://bit.ly/CP210X_USB_Driver
   ```

   </td></tr></table>

<br><br>

## Rancangan Proyek
<table>
<tr>
<th width="280">Infrastruktur</th>
<th width="280">Diagram Ilustrasi untuk ESP32</th>
<th width="280">Diagram Ilustrasi untuk NodeMCU</th>
</tr>
<tr>
<td><img src="Assets/Documentation/Diagram/Infrastructure.jpg" alt="infrastructure"></td>
<td><img src="Assets/Documentation/Diagram/Pictorial Diagram 1.jpg" alt="pd-esp32"></td>
<td><img src="Assets/Documentation/Diagram/Pictorial Diagram 2.jpg" alt="pd-nodemcu"></td>
</tr>
</table>
<table>
<tr>
<th width="840">Diagram Sistem</th>
</tr>
<tr>
<td><img src="Assets/Documentation/Diagram/Systems Diagram.jpg" alt="systems-diagram"></td>
</tr>
</table>

<br><br>

## Memindai Alamat I2C Yang Ada Pada LCD
```ino
#include <Wire.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);
  while (!Serial); // Wait for serial monitor
  Serial.println("\nI2C Scanner");
}

void loop() {
  int nDevices = 0;
  Serial.println("Scanning...");

  for (byte address = 1; address < 127; ++address) {
    // The i2c_scanner uses the return value of the Wire.endTransmission to see if a device did acknowledge to the address.
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println("  !");

      ++nDevices;
    } else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
  delay(5000); // Wait 5 seconds for next scan
}
```

<br><br>

## Pengaturan Arduino IDE
1. Buka ``` Arduino IDE ``` terlebih dahulu, kemudian buka proyek dengan cara klik ``` File ``` -> ``` Open ``` :

   <table><tr><td width="810">
   
      • ``` Final_Project_Edspert_Batch10_ESP32Project.ino ```
      
      • ``` Final_Project_Edspert_Batch10_NodeMCUProject.ino ```

   </td></tr></table><br>
   
2. Isi ``` Url Pengelola Papan Tambahan ``` di Arduino IDE

   <table><tr><td width="810">
      
      Klik ``` File ``` -> ``` Preferences ``` -> masukkan ``` Boards Manager Url ``` dengan menyalin tautan berikut :
   
      ```
      https://dl.espressif.com/dl/package_esp32_index.json
      http://arduino.esp8266.com/stable/package_esp8266com_index.json
      ```

   </td></tr></table><br>
   
3. ``` Pengaturan Board ``` di Arduino IDE

   <table>
      <tr><th>
         
      i
         
      </th><th width="780">
            
      Cara mengatur board ``` DOIT ESP32 DEVKIT V1 ```
   
      </th></tr>
      <tr><td colspan="2">

      • Klik bagian ``` Tools ``` -> ``` Board ``` -> ``` Boards Manager ``` -> Instal ``` esp32 ```.

      • Kemudian klik ``` Tools ``` -> ``` Board ``` -> ``` ESP32 Arduino ``` -> ``` DOIT ESP32 DEVKIT V1 ```.
              
      </td></tr>
   </table><br><table>
      <tr><th>
         
      ii
         
      </th><th width="775">

      Cara mengatur board ``` NodeMCU V3 ESP8266 Lolin ```
            
      </th></tr>
      <tr><td colspan="2">

      • Klik bagian ``` Tools ``` -> ``` Board ``` -> ``` Boards Manager ``` -> Instal ``` esp8266 ```.

      • Kemudian klik ``` Tools ``` -> ``` Board ``` -> ``` ESP8266 Boards ``` -> ``` NodeMCU 1.0 (ESP-12E Module) ```.
            
      </td></tr>
   </table><br>
   
4. ``` Ubah Kecepatan Papan ``` di Arduino IDE

   <table>
      <tr><th>
         
      i
         
      </th><th width="780">
            
      Cara mengubah kecepatan board ``` DOIT ESP32 DEVKIT V1 ```
   
      </th></tr>
      <tr><td colspan="2">

      Klik ``` Tools ``` -> ``` Upload Speed ``` -> ``` 115200 ```
              
      </td></tr>
   </table><br><table>
      <tr><th>
         
      ii
         
      </th><th width="775">

      Cara mengubah kecepatan board ``` NodeMCU V3 ESP8266 Lolin ```
            
      </th></tr>
      <tr><td colspan="2">

      Klik ``` Tools ``` -> ``` Upload Speed ``` -> ``` 115200 ```
            
      </td></tr>
   </table><br>
   
5. ``` Instal Pustaka ``` di Arduino IDE

   <table><tr><td width="810">
     
      Unduh semua file zip pustaka. Lalu tempelkan di: ``` C:\Users\Computer_Username\Documents\Arduino\libraries ```

   </td></tr></table><br>

6. ``` Pengaturan Port ``` di Arduino IDE

   <table><tr><td width="810">

      Klik ``` Port ``` -> pilih sesuai dengan port perangkat anda ``` (anda dapat melihatnya di Device Manager) ```
         
   </td></tr></table><br>

7. Ubah ``` Nama WiFi ```, ``` Kata Sandi WiFi ```, dan sebagainya sesuai dengan apa yang anda gunakan saat ini.<br><br>

8. Sebelum mengunggah program, silakan klik: ``` Verify ```.<br><br>

9. Jika tidak ada kesalahan dalam kode program, silakan klik: ``` Upload ```.<br><br>
    
10. Beberapa hal yang perlu anda lakukan saat menggunakan ``` board ESP32 ``` :

    <table><tr><td width="810">
       
      • Informasi ``` Arduino IDE ```: ``` Uploading... ``` -> segera tekan dan tahan tombol ``` BOOT ```.

      • Informasi ``` Arduino IDE ```: ``` Writing at .... (%) ``` -> lepaskan tombol ``` BOOT ```.

      • Tunggu sampai muncul pesan: ``` Done Uploading ``` -> ``` Program langsung dioperasikan ```.

      • Tekan tombol ``` EN (RST) ``` lalu ``` Restart ``` untuk menangani board ``` ESP32 ``` yang tidak bisa memproses ``` SC ```.

      • Jangan tekan tombol ``` BOOT ``` dan ``` EN ``` secara bersamaan karena hal ini bisa beralih ke mode ``` Unggah Firmware ```.

    </td></tr></table><br>

11. Sedangkan dalam unggah program, ``` board NodeMCU ``` lebih unggul karena umumnya sudah dapat dilakukan secara otomatis.<br><br>

12. Jika masih ada masalah saat unggah program, maka coba periksa pada bagian ``` driver ``` / ``` port ``` / ``` yang lainnya ```.

<br><br>

## Pengaturan Antares
1. Memulai Antares :

   <table><tr><td width="810">
      
   • Silakan <a href="https://beta-console.antares.id/id/signup">Registrasi</a> terlebih dahulu.

   • Selanjutnya silakan <a href="https://beta-console.antares.id/id">Masuk</a> agar dapat mengakses layanan.

   </td></tr></table><br>
   
2. Mengaktifkan access key :

   <table><tr><td width="810">
      
   • Masuk ke menu ``` Account ```.

   • Klik ``` Get Access Key ``` untuk menghasilkan access key. Proses ini hanya perlu dilakukan sekali.

   • Jika anda sudah pernah mengaktifkan access key sebelumnya, lewati langkah ini.

   </td></tr></table><br>
   
3. Membuat applications :

   <table><tr><td width="810">
      
   • Masuk ke menu ``` Applications ```.

   • Klik ``` + Create an Application ```.

   • Dalam menu ``` Add Application ```, silakan menentukan beberapa hal berikut :
      - ``` Application Name ``` -> ``` Nama Aplikasi yang akan anda buat ```.
      - ``` Application ID ``` -> ``` ID Aplikasi yang akan anda buat ```.
      - ``` Labels ``` -> tentukan sesuai dengan kebutuhan proyek.

   </td></tr></table><br>
   
4. Membuat device :

   <table><tr><td width="810">
      
   • Pastikan anda berada di menu ``` Home / Applications / Aplikasi yang anda buat ```.

   • Klik ``` + Add Device ```.

   • Nama device ini sebaiknya anda tentukan berdasarkan variabel yang ada dalam proyek.

   </td></tr></table><br>
   
5. Konfigurasi firmware :

   <table><tr><td width="810">
      
   • Pastikan anda berada di menu ``` Account ```.

   • Salin ``` Access Key ``` tersebut.

   • Tempelkan pada kode firmware, contohnya seperti ini :

   ```ino
   #define ACCESSKEY "1444e88d02acb758:b996115b1c2f6f0f"
   ```

   • Lalu, ``` Nama proyek ``` dan ``` Nama device ``` itu harus sesuai dengan yang telah dibuat tadi. Contohnya :
   
   ```ino
   #define projectName "FinalProject_Edspert10"
   #define deviceName "GreenHouse_Device1"
   ```

   </td></tr></table><br><br>

## Pengaturan MIT App Inventor
1. Buka situs resmi ``` MIT App Inventor ``` :

   <table><tr><td width="810">
   
   ```
   https://appinventor.mit.edu/
   ```

   </td></tr></table><br>
   
2. Klik ``` Create Apps! ```, kemudian masuk dengan menggunakan akun google.<br><br>

3. Klik ``` Project ``` -> lalu impor berkas yang ada di direktori ``` Smart-Green-House-Berbasis-IoT-Mobile-Apps\Src\MIT App Inventor Project\ ``` :

   <table><tr><td width="810">

   • ``` Smart_Green_House_Device_1.aia ```
   
   • ``` Smart_Green_House_Device_2.aia ```

   </td></tr></table><br>

4. Klik tombol ``` Block ``` lalu atur beberapa poin berikut ini :<br><br>
   • Isi bagian ``` initialize global url to ``` dengan format berikut :

   <table><tr><td width="810">
      
   ``` https://platform.antares.id:8443/~/antares-cse/antares-id/[YOUR_APPLICATION_NAME]/[YOUR_DEVICE_NAME]/la ```

   </td></tr></table><br>
   
   • Isi bagian ``` initialize global header to -> make a list ``` seperti berikut ini :

      <table>
      <tr><th width="810" colspan="3">
            
      ``` make a list : ```
   
      </th></tr>
      <tr><td width="270">
         
      ``` X-M2M-Origin ```.

      ``` YOUR_ACCESS_KEY ```.
   
      </td><td width="270">

      ``` Content-Type ```.

      ``` application/json;ty=4 ```.
         
      </td><td width="270">

      ``` Accept ```.

      ``` application/json ```.
   
      </td></tr>
      </table><br>

5. Kemudian klik ``` Connect ``` -> selanjutnya pilih ``` AI Companion ```.<br><br>

6. Buka ponsel cerdas anda, lalu pada ``` Google Play Store ``` cari aplikasi ``` MIT AI2 Companion ```, kemudian lakukan instalasi.<br><br>

7. Buka aplikasi ``` MIT AI2 Companion ```.<br><br>

8. Pilih metode ``` Scan QR Code ```.<br><br>

9. Arahkan ponsel cerdas anda ke area ``` QR Code ``` yang ada di situs ``` MIT App Inventor ```.<br><br>

10. Jika ingin mengoperasikan 2 sistem dalam waktu yang bersamaan, maka anda dapat melakukannya dengan 2 device yang berbeda agar sistem tidak bentrok.

<br><br>

## Memulai
1. Unduh dan ekstrak repositori ini.<br><br>
   
2. Pastikan anda memiliki komponen elektronik yang diperlukan.<br><br>
   
3. Pastikan komponen anda telah dirancang sesuai dengan diagram.<br><br>
   
4. Konfigurasikan perangkat anda menurut pengaturan di atas.<br><br>

5. Selamat menikmati [Selesai].

<br><br>

## Sorotan
<table>
<tr>
<th width="420">Perangkat Pemantauan menggunakan ESP32</th>
<th width="420">Platform Antares untuk ESP32</th>
</tr>
<tr>
<td><img src="Assets/Documentation/Experiment/Percobaan Device 1 ESP32.jpg" alt="product-esp32"></td>
<td><img src="Assets/Documentation/Experiment/Percobaan Device 1 Antares.jpg" alt="antaresplatform-esp32"></td>
</tr>
<tr>
<th colspan="2">Konfigurasi MIT App Inventor untuk ESP32</th>
</tr>
<tr>
<td colspan="2" width="840"><img src="Assets/Documentation/Experiment/MIT App Inventor ESP32.jpg" alt="mitappinventor-device1"></td>
</tr>
</table>
<table>
<tr>
<th colspan="2">Perangkat Pengendalian menggunakan NodeMCU</th>
</tr>
<tr>
<td width="420"><img src="Assets/Documentation/Experiment/Percobaan Device 2 NodeMCU 1.jpg" alt="product-nodemcu-ac1"></td>
<td width="420"><img src="Assets/Documentation/Experiment/Percobaan Device 2 NodeMCU 2.jpg" alt="product-nodemcu-ac2"></td>
</tr>
<tr>
<th colspan="2">Platform Antares untuk NodeMCU</th>
</tr>
<tr>
<td colspan="2" width="840"><img src="Assets/Documentation/Experiment/Percobaan Device 2 Antares.jpg" alt="antaresplatform-nodemcu"></td>
</tr>
<tr>
<th colspan="2">Konfigurasi MIT App Inventor untuk NodeMCU</th>
</tr>
<tr>
<td colspan="2" width="840"><img src="Assets/Documentation/Experiment/MIT App Inventor NodeMCU.jpg" alt="mitappinventor-device2"></td>
</tr>
</table>
<table>
<tr>
<th colspan="2">Simulasi Pemantauan dengan Aplikasi Mobile</th>
</tr>
<tr>
<td width="420"><img src="Assets/Documentation/Experiment/Percobaan Device 1 MIT App Inventor.jpg" alt="simulation-1"></td>
<td width="420"><img src="Assets/Documentation/Experiment/Percobaan Device 2 MIT App Inventor.jpg" alt="simulation-2"></td>
</tr>
</table>

<br><br>

## Apresiasi
Jika karya ini bermanfaat bagi anda, maka dukunglah karya ini sebagai bentuk apresiasi kepada penulis dengan mengklik tombol ``` ⭐Bintang ``` di bagian atas repositori.

<br><br>

## Penafian
Aplikasi ini dibuat dengan menyertakan sumber-sumber dari pihak ketiga. Pihak ketiga di sini adalah penyedia layanan, yang layanannya berupa pustaka, kerangka kerja, dan lain-lain. Saya ucapkan terima kasih banyak atas layanannya. Telah terbukti sangat membantu dan dapat diimplementasikan.

<br><br>

## LISENSI
LISENSI MIT - Hak Cipta © 2023 - Devan C. M. Wijaya, S.Kom

Dengan ini diberikan izin tanpa biaya kepada siapa pun yang mendapatkan salinan perangkat lunak ini dan file dokumentasi terkait perangkat lunak untuk menggunakannya tanpa batasan, termasuk namun tidak terbatas pada hak untuk menggunakan, menyalin, memodifikasi, menggabungkan, mempublikasikan, mendistribusikan, mensublisensikan, dan/atau menjual salinan Perangkat Lunak ini, dan mengizinkan orang yang menerima Perangkat Lunak ini untuk dilengkapi dengan persyaratan berikut:

Pemberitahuan hak cipta di atas dan pemberitahuan izin ini harus menyertai semua salinan atau bagian penting dari Perangkat Lunak.

DALAM HAL APAPUN, PENULIS ATAU PEMEGANG HAK CIPTA DI SINI TETAP MEMILIKI HAK KEPEMILIKAN PENUH. PERANGKAT LUNAK INI DISEDIAKAN SEBAGAIMANA ADANYA, TANPA JAMINAN APAPUN, BAIK TERSURAT MAUPUN TERSIRAT, OLEH KARENA ITU JIKA TERJADI KERUSAKAN, KEHILANGAN, ATAU LAINNYA YANG TIMBUL DARI PENGGUNAAN ATAU URUSAN LAIN DALAM PERANGKAT LUNAK INI, PENULIS ATAU PEMEGANG HAK CIPTA TIDAK BERTANGGUNG JAWAB, KARENA PENGGUNAAN PERANGKAT LUNAK INI TIDAK DIPAKSAKAN SAMA SEKALI, SEHINGGA RISIKO ADALAH MILIK ANDA SENDIRI.
