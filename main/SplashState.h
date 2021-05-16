#ifndef __SPLASH_SCREEN_STATE
#define __SPLASH_SCREEN_STATE

#include <Adafruit_GFX.h>               // Graphics
#include "Adafruit_ThinkInk.h"          // E-Paper Display Driver
#include <Adafruit_ImageReader_EPD.h>   // E-Paper Display Image-reading

#include "main.h"
#include "ScreenManager.h"
#include "State.h"
#include "Key.h"
#include "Images.h"

class SplashScreenState : public State {

public:
	SplashScreenState(SplashScreenState&) = delete; // Copy prohibited
    void operator=(const SplashScreenState&) = delete; // Assignment prohibited
    SplashScreenState& operator=(SplashScreenState&&) = delete; // Move assignment
	// Splash State Deconstructor
	~SplashScreenState();

	// Splash State Instance Function
	static SplashScreenState* Instance() {
		if( _instance == 0 ) {
			_instance = new SplashScreenState();
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
	static SplashScreenState* _instance;
    bool visualStateChanged;

	SplashScreenState();
	void operator delete(void*) {}; // such that its not deleted accidentally
};

#endif