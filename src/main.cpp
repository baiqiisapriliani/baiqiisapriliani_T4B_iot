#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include "DHT.h"

#define DHTPIN 27    
#define DHTTYPE DHT22  
DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "rindaii";
const char* password = "hellostranger";

unsigned long previousMillis = 0;
const long interval = 5000;

void setup() {
  Serial.begin(115200);
  delay(1000);  // kasih waktu buat serial nyala

  WiFi.begin(ssid, password);
  Serial.print("Menghubungkan ke WiFi");

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    Serial.print(".");
    retry++;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println(" Terhubung!");
    Serial.print("IP ESP32: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println(" Gagal terhubung ke WiFi.");
  }

  dht.begin();
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    float h = round(dht.readHumidity());
    float t = round(dht.readTemperature());

    if (isnan(h) || isnan(t)) {
      Serial.println("Gagal membaca dari sensor DHT!");
      return;
    }

    float hic = dht.computeHeatIndex(t, h, false);

    if (WiFi.status() == WL_CONNECTED) {
      HTTPClient http;
      String url = "http://f1c4-139-195-134-207.ngrok-free.app/api/posts";
      http.begin(url);
      http.addHeader("Content-Type", "application/json");

      String payload = "{\"nama_sensor\":\"Sensor GD\", \"nilai1\":" + String(h) + ", \"nilai2\":" + String(t) + "}";
      Serial.println("Payload: " + payload);

      int httpResponseCode = http.POST(payload);

      Serial.print("Kode respons HTTP: ");
      Serial.println(httpResponseCode);

      if (httpResponseCode == 200 || httpResponseCode == 201) {
        String response = http.getString();
        Serial.println("Respons dari server:");
        Serial.println(response);
      } else {
        Serial.println("Gagal mengirim data");
      }

      http.end();
    } else {
      Serial.println("WiFi tidak terhubung, tidak mengirim data.");
    }
  }
}
