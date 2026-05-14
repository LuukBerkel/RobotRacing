#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include "camera.hpp"
#include "http_server.hpp"

#define WIFI_SSID "**********"
#define WIFI_PASSWORD "**********"
#define DRIVE_TIMEOUT_MS 2000

Camera camera;
Drive drive;
HTTPServer server;

void setup() {
  Serial.begin(115200);

  Serial.println("Initializing drive system...");
  drive.init(DRIVE_TIMEOUT_MS); 
  Serial.println("Drive system initialized successfully");

  Serial.println("Initializing camera...");
  esp_err_t err = camera.init(FRAMESIZE_SVGA, 10);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(10000);
    esp_restart();
  }
  Serial.println("Camera initialized successfully");

  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setSleep(false);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("WiFi connected");

  Serial.println(psramFound() ? "YES" : "NO");
  Serial.println(ESP.getFreeHeap());

  Serial.println("Starting HTTP server...");
  server.init(&camera, &drive);
  server.startServer();

  Serial.println("HTTP server running! Use 'http://"
    + WiFi.localIP().toString()
    + "/stream' to view the camera stream");
}

void loop() {
  // Do nothing. Everything is done in another task by the http server
  delay(10000);
}


