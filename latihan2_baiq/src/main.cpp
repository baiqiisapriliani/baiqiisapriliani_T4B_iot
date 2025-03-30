#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>


#include "DHT.h"




#define DHTPIN 27    
#define DHTTYPE DHT22  




DHT dht(DHTPIN, DHTTYPE);






// Ganti dengan kredensial WiFi Anda
const char* ssid = "Wokwi-GUEST";
const char* password = "";


unsigned long previousMillis = 0;
const long interval = 5000;  // Interval 5 detik (5000 ms)


void setup() {
  Serial.begin(115200);
 
  // Hubungkan ke WiFi
  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Terhubung!");








  dht.begin();
 
  // Tunggu sebentar agar koneksi stabil
  delay(1000);
}


void loop() {
  unsigned long currentMillis = millis();


  // Lakukan POST setiap interval yang telah ditentukan
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float h = round(dht.readHumidity());
    // Read temperature as Celsius (the default)
    float t = round(dht.readTemperature());
 
 
    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    // Inisialisasi HTTPClient
    HTTPClient http;
    String url = "http://1eec-175-45-191-10.ngrok-free.app/api/posts"; // Ganti dengan URL ngrok yang benar


    http.begin(url);  // Menggunakan HTTP, bukan HTTPS
    http.addHeader("Content-Type", "application/json");




String payload = "{\"nama_sensor\":\"Sensor GD\", \"nilai1\":" + String(h) + ", \"nilai2\":" + String(t) + "}";


Serial.println(payload);  // Untuk melihat apakah payload sudah terbentuk dengan benar


    // Kirim POST request
    int httpResponseCode = http.POST(payload);
   
    // Tampilkan kode respons HTTP
    Serial.print("Kode respons HTTP: ");
    Serial.println(httpResponseCode);


    // Tampilkan respons dari server jika request berhasil
    if (httpResponseCode == 200 || httpResponseCode == 201) {
      String response = http.getString();
      Serial.println("Respons dari server:");
      Serial.println(response);
    } else {
      Serial.println("Gagal mengirim data");
    }


    // Tutup koneksi HTTP
    http.end();
  }
}
=======
const int trigPin = 5;
const int echoPin = 18;
//define sound speed in cm/uS
#define SOUND_SPEED 0.034
#define CM_TO_INCH 0.393701
long duration;
float distanceCm;
float distanceInch;
void setup() {
 Serial.begin(115200); // Starts the serial communication
 pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
 pinMode(echoPin, INPUT); // Sets the echoPin as an Input
}
void loop() {
 // Clears the trigPin
 digitalWrite(trigPin, LOW);
 delayMicroseconds(2);
 // Sets the trigPin on HIGH state for 10 micro seconds
 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10);
 digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
 duration = pulseIn(echoPin, HIGH);
  // Calculate the distance
 distanceCm = duration * SOUND_SPEED/2;
  // Convert to inches
 distanceInch = distanceCm * CM_TO_INCH;
  // Prints the distance in the Serial Monitor
 Serial.print("Distance (cm): ");
 Serial.println(distanceCm);
 // Serial.print("Distance (inch): ");
 // Serial.println(distanceInch);
  delay(1000);
}
>>>>>>> 615cd1b2c129359fad37432e29569f5b3c407247
