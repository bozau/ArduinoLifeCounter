#include "SplashState.h"

// Setup the instance to not equal anything
SplashScreenState* SplashScreenState::_instance = 0;

// Splash State Constructor
SplashScreenState::SplashScreenState() {
}

// Splash State Deconstructor
SplashScreenState::~SplashScreenState() {
}

// Splash Screen OnLostDevice Function
void SplashScreenState::OnLostDevice() {
}

void SplashScreenState::OnResetDevice() {
}

// On Load State
void SplashScreenState::OnLoadState() {
    // Enable Wifi
    //WifiManager::Instance()->EnableWifi();
}

// On Leave State
void SplashScreenState::OnLeaveState() {
    // Disable Wifi
    //WifiManager::Instance()->DisableWifi();
}

// Splash State DoInput Function
void SplashScreenState::DoInput() {

}

// Splash State DoUpdate Function
void SplashScreenState::DoUpdate() {
    
}

// Splash State DoRender Function
void SplashScreenState::DoRender() {
    if( visualStateChanged ) {
        //ImageReturnCode stat; // Status from image-reading functions
        // Render our splash screen
        ScreenManager::Instance()->ClearScreen();

            //stat = reader.drawBMP(START_SPLASH, ScreenManager::Instance()->GetScreen(), 0, 0);
            //reader.printStatus(stat); // How'd we do?  

        ScreenManager::Instance()->GetScreen().setTextColor(EPD_WHITE);
        if( !WifiManager::Instance()->WifiEnabled() ) { // Don't render Wifi details if its turned off
            ScreenManager::Instance()->GetScreen().setCursor(10,95);

            ScreenManager::Instance()->GetScreen().print("WIFI: ");
            //ScreenManager::Instance()->GetScreen().print(WIFI_SSID);
            ScreenManager::Instance()->GetScreen().print(" - IP: ");
            //ScreenManager::Instance()->GetScreen().print(IP);
        }

        ScreenManager::Instance()->GetScreen().setCursor(20,107);
        ScreenManager::Instance()->GetScreen().print("Commander");
        ScreenManager::Instance()->GetScreen().fillTriangle(135, 111, 145, 111, 140, 121, EPD_WHITE);
        ScreenManager::Instance()->GetScreen().setCursor(175,107);
        ScreenManager::Instance()->GetScreen().print("Modern");
        ScreenManager::Instance()->GetScreen().fillTriangle(255, 111, 265, 111, 260, 121, EPD_WHITE);

        ScreenManager::Instance()->ScreenBufferReady();
    }
}
