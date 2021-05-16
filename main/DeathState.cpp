#include "DeathState.h"

// Setup the instance to not equal anything
DeathScreenState* DeathScreenState::_instance = 0;

// Splash State Constructor
DeathScreenState::DeathScreenState() {

}

// Splash State Deconstructor
DeathScreenState::~DeathScreenState() {
}

// Splash Screen OnLostDevice Function
void DeathScreenState::OnLostDevice() {

	

}

void DeathScreenState::OnResetDevice() {



}

// On Load State
void DeathScreenState::OnLoadState() {


}

// On Leave State
void DeathScreenState::OnLeaveState() {


}

// Splash State DoInput Function
void DeathScreenState::DoInput() {


}

// Splash State DoUpdate Function
void DeathScreenState::DoUpdate() {


}

// Splash State DoRender Function
void DeathScreenState::DoRender() {
    // Render our splash screen
    ScreenManager::Instance()->ClearScreen();

    ImageReturnCode stat; // Status from image-reading functions

        //reader.drawBMP(DEATH_SPLASH, &ScreenManager::Instance()->GetScreen(), 0, 0);
        //reader.printStatus(stat); // How'd we do?  

    ScreenManager::Instance()->ScreenBufferReady();
}
