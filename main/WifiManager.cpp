#include "WifiManager.h"

// Set the game state
WifiManager* WifiManager::_instance = 0;

// Game State Constructor
WifiManager::WifiManager() {
    wifiEnabled = false;
    ssid = "bozau-lifecounter";
    ssid_pw = "password";
    wifiOffTime = 60000;
    //local_ip(192, 168, 1, 1);
    //gateway(192, 168, 1, 2);
    //subnet(255, 255, 255, 0);

}

// Game State Deconstructor
WifiManager::~WifiManager() {
}

void WifiManager::EnableWifi() {
    //  WiFi.softAPConfig(local_ip, gateway, subnet);
    // WiFi.softAP( ssid, ssid_pw );
    // ip = WiFi.softAPIP();
    // wifiEnabled = true;
}

void WifiManager::DisableWifi() {
    if( wifiEnabled ) {
        //WiFi.softAPdisconnect(true);
        wifiEnabled = false;
    }
}

bool WifiManager::WifiEnabled() {
    return wifiEnabled;
}

String WifiManager::GetIP() {
    return ip;
}

String WifiManager::GetSSID() {
    return ssid;
}

// // Web Server
// WiFiServer server(80);
// String header; // Variable to store the HTTP request


//   // Create wireless AP
//   WiFi.softAPConfig(local_IP, gateway, subnet);
//   Serial.print( "Setting AP (Access Point)…" );
//   WiFi.softAP( WIFI_SSID, WIFI_PSK );

//   IP = WiFi.softAPIP();
//   Serial.print("Access Point IP address: ");
//   Serial.println( IP );


//   // Start the Wifi connection
//   server.begin();
//   Serial.println("Wireless Access Point online.");

// // Turn off the wifi after WIFI_OFF_TIME (should be enough time to configure) and save battery
//   if( ( timeSinceStartup > WIFI_OFF_TIME && wifiEnabled ) || setupComplete ) {
//     if( wifiOn ) {
//       while( WiFi.softAPdisconnect(true) ) {
//         Serial.println("Turning WiFi off.");
//         delay( 50 );
//       }
//       wifiOn = false;
//       Serial.println("Wifi has been turned off.");
//     }

//     // Enter light sleep after no choice has been made and wifi's off
//     if( !setupComplete ) {
//       renderSplash();
//       delay( 100 );
//       Serial.print( "Sleep Status: " );
//       Serial.println( putToLightSleep() );
//     }
//   }
