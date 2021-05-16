#include "GameState.h"

// Setup the instance to not equal anything
GameState* GameState::_instance = 0;

// Splash State Constructor
GameState::GameState() {

}

// Splash State Deconstructor
GameState::~GameState() {
}


void GameState::StartGame( int _life, int _commanders, int _gameType ) {
  life = _life;
  commanderDamage.clear();
  commanderDamage.resize( _commanders, 0 );
  gameType = _gameType;

  // Remove commanders from the list
  if( static_cast<int>( commanders.size() ) > 0 ) {
    commanders.clear();
  }
}

int GameState::AlterLifeTotal( int delta ) {
}

int GameState::AlterInfectTotal( int delta ) {
}

int GameState::AlterCommanderDamage( int delta, int commander ) {
}

int GameState::AlterCommanderCast( int delta, int commander ) {
}


int GameState::GetOpponentCount() {

}

int GameState::GetLifeTotal() {

}

int GameState::GetCommanderDamage( int commander ) {
}

int GameState::GetCommanderCast( int commander ) {
}

std::vector< Commander > GameState::GetCommanders() {
  return commanders;
}

int GameState::GetGameType() {

}

void GameState::AddCommander( Commander& commander ) {
  commanders.push_back( commander );
}
