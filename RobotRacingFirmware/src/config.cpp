#include "config.hpp"

#define PREFERENCES_NAMESPACE "robot_racing"
#define PREFERENCES_NAME_KEY "name"


void Config::init(String robotSSID, String robotPassword, String fallbackName) {
  this->robotSSID = robotSSID;
  this->robotPassword = robotPassword;
  this->fallbackName = fallbackName;

  Preferences preferences;
  preferences.begin(PREFERENCES_NAMESPACE, true);
  this->robotName = preferences.getString(PREFERENCES_NAME_KEY, fallbackName);
  if (!preferences.isKey(PREFERENCES_NAME_KEY)){
    wifiManager.resetSettings();
  } 
  preferences.end();

  customName = new WiFiManagerParameter(
    "name",
    "Robot Name",
    this->fallbackName.c_str(),
    32
  );
}

void Config::setupWifi() {
  wifiManager.addParameter(customName);
  wifiManager.autoConnect(robotSSID.c_str(), robotPassword.c_str());
  if (strcmp(customName->getValue(), fallbackName.c_str()) == 0) {
    Serial.println("Invalid robot name. Name cannot be the fallback name.");
    return;
  }

  Preferences preferences;
  preferences.begin(PREFERENCES_NAMESPACE, false);
  preferences.putString(PREFERENCES_NAME_KEY, customName->getValue());
  preferences.end();

  this->robotName = customName->getValue();
}

String Config::getRobotName() {
  return robotName;
}