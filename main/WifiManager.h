#ifndef __WIFI_MANAGER
#define __WIFI_MANAGER

#include <Arduino.h>
#include <SPI.h>
#include <WiFi.h>                       // Wifi driver
#include <WiFiClient.h>                 // Wireless Client
#include <WiFiAP.h>                     // Wireless Access Poin
#include "HelperFunctions.h"

class WifiManager {

public:
	WifiManager(WifiManager&) = delete; // Copy prohibited
    void operator=(const WifiManager&) = delete; // Assignment prohibited
    WifiManager& operator=(WifiManager&&) = delete; // Move assignment

  // Game State Deconstructor
  ~WifiManager();

  // Game State Instance Function
  static WifiManager* Instance() {
    if( _instance == 0 ) {
      _instance = new WifiManager();
    }
    return _instance;
  }

  void EnableWifi();
  void DisableWifi();

  bool WifiEnabled();
  String GetIP();
  String GetSSID();

private:
  // Game State Manager instance
  static WifiManager* _instance;
  bool wifiEnabled;
  String ip;
  String ssid;
  String ssid_pw;
  int wifiOffTime;

  // IPAddress local_ip;
  // IPAddress gateway;
  // IPAddress subnet;

  WifiManager();
	void operator delete(void*) {}; // such that its not deleted accidentally
};

#endif // __SCREEN_MANAGER
