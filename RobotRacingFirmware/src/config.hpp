#pragma once

#include <Arduino.h>
#include <Preferences.h>
#include <WiFiManager.h>

class Config {
public:
  void init(String robotSSID, String robotPassword, String fallbackName);

  /**
   * @brief Get the name of the robot from config.
   * @return The name of the robot, or a fallback name if not set.
   */
  String getRobotName();

  /**
   * @brief Sets up WiFi connection and configuration.
   * This should be called in the main loop to allow the WiFiManager to process connection events and configuration changes.
   */
  void setupWifi();
private:
  Preferences preferences;

  WiFiManager wifiManager;
  WiFiManagerParameter* customName;

  String robotName;
  String fallbackName;
  String robotSSID;
  String robotPassword;
};