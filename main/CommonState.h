#ifndef __COMMON_SCREEN_STATE
#define __COMMON_SCREEN_STATE

#include "main.h"
#include "Structs.h"
#include "ScreenManager.h"
#include "State.h"
#include "Key.h"
#include "Images.h"
#include "HelperFunctions.h"


class CommonScreenState : public State {

public:
	CommonScreenState(CommonScreenState&) = delete; // Copy prohibited
    void operator=(const CommonScreenState&) = delete; // Assignment prohibited
    CommonScreenState& operator=(CommonScreenState&&) = delete; // Move assignment

	// Splash State Deconstructor
	~CommonScreenState();

	// Splash State Instance Function
	static CommonScreenState* Instance() {
		if( _instance == 0 ) {
			_instance = new CommonScreenState();
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
	static CommonScreenState* _instance;

	GameType* gameTypes;
	
	CommonScreenState();
	void operator delete(void*) {}; // such that its not deleted accidentally

};

#endif
