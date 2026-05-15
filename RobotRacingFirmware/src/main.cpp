#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiManager.h>
#include <Preferences.h>
#include <string.h>
#include "camera.hpp"
#include "drive.hpp"
#include "http_server.hpp"
#include "config.hpp"


#define CONFIG_SSID "RobotRacing"
#define CONFIG_PASSWORD "RobotRacing"
#define DRIVE_TIMEOUT_MS 2000
#define FALLBACK_NAME "Example"

Camera camera;
Drive drive;
HTTPServer server;
Config config;

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
  config.init(CONFIG_SSID, CONFIG_PASSWORD, FALLBACK_NAME);
  config.setupWifi();
  Serial.println("WiFi connected");


  Serial.println("Starting HTTP server...");
  server.init(&camera, &drive, config.getRobotName());
  server.startServer();

  Serial.println("HTTP server running! Use 'http://"
    + WiFi.localIP().toString()
    + "/stream' to view the camera stream");
}

void loop() {
  // Do nothing. Everything is done in another task by the http server
  delay(10000);
}