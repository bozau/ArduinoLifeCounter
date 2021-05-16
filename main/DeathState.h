#ifndef __DEATH_SCREEN_STATE
#define __DEATH_SCREEN_STATE

#include "main.h"
#include "Structs.h"
#include "ScreenManager.h"
#include "State.h"
#include "Key.h"
#include "Images.h"

class DeathScreenState : public State {

public:
	DeathScreenState(DeathScreenState&) = delete; // Copy prohibited
    void operator=(const DeathScreenState&) = delete; // Assignment prohibited
    DeathScreenState& operator=(DeathScreenState&&) = delete; // Move assignment
	// Splash State Deconstructor
	~DeathScreenState();

	// Splash State Instance Function
	static DeathScreenState* Instance() {
		if( _instance == 0 ) {
			_instance = new DeathScreenState();
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
	// Instance Pointer for the state
	static DeathScreenState* _instance;

	DeathScreenState();
	void operator delete(void*) {}; // such that its not deleted accidentally
};

#endif
