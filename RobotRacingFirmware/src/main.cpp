#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include "camera.hpp"
#include "http_server.hpp"

const char *ssid = "**********";
const char *password = "**********";

Camera camera;
HTTPServer server;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing camera...");

  esp_err_t err = camera.init(FRAMESIZE_QVGA, 10);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    delay(10000);
    esp_restart();
  }
  Serial.println("Camera initialized successfully");

  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  WiFi.setSleep(false);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  Serial.println("Starting HTTP server...");
  server.init(camera);
  server.startServer();

  Serial.println("HTTP server running! Use 'http://"
    + WiFi.localIP().toString()
    + "/stream' to view the camera stream");
}

void loop() {
  // Do nothing. Everything is done in another task by the http server
  delay(10000);
}


