#ifndef __GAME_STATE
#define __GAME_STATE

#include "Arduino.h"
#include <vector>

#include "Structs.h"

class GameState {

public:
	GameState(GameState&) = delete; // Copy prohibited
    void operator=(const GameState&) = delete; // Assignment prohibited
    GameState& operator=(GameState&&) = delete; // Move assignment

	// Splash State Deconstructor
	~GameState();

	// Splash State Instance Function
	static GameState* Instance() {
		if( _instance == 0 ) {
			_instance = new GameState();
		}
		return _instance;
	}

	void StartGame( int life, int commanders, int gameType );

	int AlterLifeTotal( int delta );
	int AlterInfectTotal( int delta );
	int AlterCommanderDamage( int delta, int commander );
	int AlterCommanderCast( int delta, int commander );

	int GetOpponentCount();
	int GetLifeTotal();
	int GetCommanderDamage( int commander );
	int GetCommanderCast( int commander );
	int GetGameType();
  	std::vector< Commander > GetCommanders();

	void AddCommander( Commander& commander );

private:
	static GameState* _instance;
	int gameType;
	int life;
	int infect;
	std::vector< int > commanderDamage;
	std::vector< Commander > commanders;
	
	GameState();
	void operator delete(void*) {}; // such that its not deleted accidentally

};

#endif
