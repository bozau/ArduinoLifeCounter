#include "CommanderState.h"

// Setup the instance to not equal anything
CommanderScreenState* CommanderScreenState::_instance = 0;

// Splash State Constructor
CommanderScreenState::CommanderScreenState() {
  gameTypes = new GameType();
}

// Splash State Deconstructor
CommanderScreenState::~CommanderScreenState() {
  delete gameTypes;
}

// Splash Screen OnLostDevice Function
void CommanderScreenState::OnLostDevice() {

}

void CommanderScreenState::OnResetDevice() {

}

// On Load State
void CommanderScreenState::OnLoadState() {
  GameState::Instance()->StartGame( 40, 3, gameTypes->Commander ); // Todo, allow choice of opponents
  Commander commander = Commander{ "Muldrotha", 0, "2{B}{G}{U}" };
  GameState::Instance()->AddCommander( commander );
}

// On Leave State
void CommanderScreenState::OnLeaveState() {

}

// Splash State DoInput Function
void CommanderScreenState::DoInput() {

}

// Splash State DoUpdate Function
void CommanderScreenState::DoUpdate() {

}

// Splash State DoRender Function
void CommanderScreenState::DoRender() {
  Serial.print("Life Total: ");
  Serial.println( GameState::Instance()->GetLifeTotal() );

  ScreenManager::Instance()->ClearScreen();

  // Title
  ScreenManager::Instance()->GetScreen().setFont(&Sail_Regular22pt7b); // Requires a 3px buffer on Y
  ScreenManager::Instance()->GetScreen().setTextColor(EPD_BLACK);
  ScreenManager::Instance()->GetScreen().setCursor(42,26 + 2);
  ScreenManager::Instance()->GetScreen().print("Commander");

  // Commander Damage

  // Life Value
  ScreenManager::Instance()->GetScreen().setTextColor(EPD_BLACK);
  Serial.print( "Number of 1's in current life total: " );
  Serial.println( CharHowManyTimes( String( GameState::Instance()->GetLifeTotal() ), "1" ) );

  // Move cursor if in single or double digits
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

  // Infect

  // Commander
  

  ScreenManager::Instance()->ScreenBufferReady();
}


