#include <Arduino.h>
#include <HTTPClient.h>
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include "main.hpp"

#define PIN_BUTTON 27

// WiFi接続情報
const char* ssid = SSID;
const char* password = PASSWORD;
const char* access_token = LINE_ACCESS_TOKEN;
const char* url_broadcast = "https://api.line.me/v2/bot/message/broadcast";

// NTPクライアント
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600 * 9, 60000);  // 日本時間 (UTC+9)

// WiFi接続関数
void connect_wifi() {
  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

// スリープ関数 (22:00～6:00の8時間)
void sleep_if_night() {
  int currentHour = timeClient.getHours();

  if (currentHour >= 22 || currentHour < 6) {
    Serial.println("Entering Deep Sleep Mode for 8 hours");
    esp_deep_sleep(8 * 3600 * 1000000ULL);  // 8時間スリープ
  }
}

// Web APIアクセス関数
void line_notifiy() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(url_broadcast);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("Authorization", String("Bearer ") + access_token);

    String json_data = "{\"messages\":[{\"type\":\"text\",\"text\":\"おや、誰か来たかも？\"}]}";
    int httpResponseCode = http.POST(json_data);

    if (httpResponseCode > 0) {
      Serial.print("Response Code: ");
      Serial.println(httpResponseCode);
      String response = http.getString();
      Serial.println(response);
    } else {
      Serial.print("Error: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }
}

// メイン処理
void setup() {
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  Serial.begin(115200);
  connect_wifi();

  timeClient.begin();

  timeClient.update();
  Serial.print("Current time: ");
  Serial.println(timeClient.getFormattedTime());
}

void loop() {
  static bool keep = false;

  bool sw = digitalRead(PIN_BUTTON);

  // ドアホンコール後、A接点は5s継続する
  if (keep) {
    if (sw == HIGH) {
      keep = false;
    }
  } else {
    if (sw == LOW) {
      line_notifiy();
      delay(3000);
      keep = true;
    }
  }

  sleep_if_night();
}
