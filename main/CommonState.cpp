#include "CommonState.h"

// Setup the instance to not equal anything
CommonScreenState* CommonScreenState::_instance = 0;

// Splash State Constructor
CommonScreenState::CommonScreenState() {
  gameTypes = new GameType();
}

// Splash State Deconstructor
CommonScreenState::~CommonScreenState() {
}

// Splash Screen OnLostDevice Function
void CommonScreenState::OnLostDevice() {

}

void CommonScreenState::OnResetDevice() {
}

// On Load State
void CommonScreenState::OnLoadState() {
}

// On Leave State
void CommonScreenState::OnLeaveState() {

}

// Splash State DoInput Function
void CommonScreenState::DoInput() {

}

// Splash State DoUpdate Function
void CommonScreenState::DoUpdate() {

}

// Splash State DoRender Function
void CommonScreenState::DoRender() {
    Serial.print("Life Total: ");
    Serial.println( GameState::Instance()->GetLifeTotal() );

    ScreenManager::Instance()->ClearScreen();

    // Title
    ScreenManager::Instance()->GetScreen().setFont(&Sail_Regular22pt7b); // Requires a 3px buffer on Y
    ScreenManager::Instance()->GetScreen().setTextColor(EPD_BLACK);
    if( GameState::Instance()->GetGameType() == gameTypes->Modern ) {
        ScreenManager::Instance()->GetScreen().setCursor(62,26 + 2);
        ScreenManager::Instance()->GetScreen().print("Modern");
    } else if( GameState::Instance()->GetGameType() == gameTypes->Standard ) {
        ScreenManager::Instance()->GetScreen().setCursor(50,26 + 2);
        ScreenManager::Instance()->GetScreen().print("Standard");
    } else if( GameState::Instance()->GetGameType() == gameTypes->Draft ) {
        ScreenManager::Instance()->GetScreen().setCursor(90,26 + 2);
        ScreenManager::Instance()->GetScreen().print("Draft");
    }

    // Life Value
    ScreenManager::Instance()->GetScreen().setTextColor(EPD_BLACK);
    Serial.print( "Number of 1's in current life total: " );
    Serial.println( CharHowManyTimes( String( GameState::Instance()->GetLifeTotal() ), "1" ) );

    // Move cursor if in single or double digits // TODO NEED TO FIX FOR NON COMMANDER
    int cursorXInitPos = 85;
    if( 0 <= GameState::Instance()->GetLifeTotal() < 10 ) {
        cursorXInitPos = cursorXInitPos + 15;
    } else if( 100 <= GameState::Instance()->GetLifeTotal() || GameState::Instance()->GetLifeTotal() <= -10 ) {
        cursorXInitPos = cursorXInitPos - 30;
    } else {
        cursorXInitPos = cursorXInitPos - 30;
    }

    // Due to 1's being smaller in the text we need to move the cursor
    int cursorXPosInc = 16 * CharHowManyTimes( String( GameState::Instance()->GetLifeTotal() ), "1" ); // Position increase amount if 1's exist

    ScreenManager::Instance()->GetScreen().setFont(&JosefinSans_VariableFont_wght60pt7b); // Requires a 22px buffer on Y and buffer on X
    ScreenManager::Instance()->GetScreen().setCursor( cursorXInitPos + cursorXPosInc, 117 );

    ScreenManager::Instance()->GetScreen().print( GameState::Instance()->GetLifeTotal() );
    ScreenManager::Instance()->ScreenBufferReady();
}


