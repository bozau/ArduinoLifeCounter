#ifndef __COMMANDER_SCREEN_STATE
#define __COMMANDER_SCREEN_STATE

#include "main.h"
#include "Structs.h"
#include "ScreenManager.h"
#include "State.h"
#include "Key.h"
#include "Images.h"


class CommanderScreenState : public State {

public:
	CommanderScreenState(CommanderScreenState&) = delete; // Copy prohibited
    void operator=(const CommanderScreenState&) = delete; // Assignment prohibited
    CommanderScreenState& operator=(CommanderScreenState&&) = delete; // Move assignment
	// Splash State Deconstructor
	~CommanderScreenState();

	// Splash State Instance Function
	static CommanderScreenState* Instance() {
		if( _instance == 0 ) {
			_instance = new CommanderScreenState();
		}
		return _instance;
	}
	
	// Splash State DoInput Function
	void DoInput();

	// Splash State DoUpdate Function
	void DoUpdate();

	// Splash State DoRender Function
	void DoRender();

	// Splash State OnLostDevice Function
	void OnLostDevice();

	// Splash State OnResetDevice Function
	void OnResetDevice();

	// On Load State
	void OnLoadState();

	// On Leave State
	void OnLeaveState();

private:
	static CommanderScreenState* _instance;

	GameType* gameTypes;

	CommanderScreenState();
	void operator delete(void*) {}; // such that its not deleted accidentally

};

#endif
