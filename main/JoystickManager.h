#ifndef __JOYSTICK_MANAGER
#define __JOYSTICK_MANAGER

#include <Arduino.h>
#include <map>
#include "Structs.h"
#include "State.h"

class JoystickManager {

public:

	JoystickManager(JoystickManager&) = delete; // Copy prohibited
    void operator=(const JoystickManager&) = delete; // Assignment prohibited
    JoystickManager& operator=(JoystickManager&&) = delete; // Move assignment

  // Game State Deconstructor
  ~JoystickManager();

  // Game State Instance Function
  static JoystickManager* Instance() {
    if( _instance == 0 ) {
      _instance = new JoystickManager;
    }
    return _instance;
  }

private:
  // Game State Manager instance
  static JoystickManager* _instance;
  int XAxisPin; //10
  int YAxisPin; //7
  
  // Game State Constructor
  JoystickManager();
	void operator delete(void*) {}; // such that its not deleted accidentally
};

#endif // __STATE_MANAGER
