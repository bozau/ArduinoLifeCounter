#ifndef __STATE_MANAGER
#define __STATE_MANAGER

#include <Arduino.h>
#include <map>
#include "State.h"

class StateManager {

public:

	StateManager(StateManager&) = delete; // Copy prohibited
    void operator=(const StateManager&) = delete; // Assignment prohibited
    StateManager& operator=(StateManager&&) = delete; // Move assignment

  // Game State Deconstructor
  ~StateManager();

  // Game State Instance Function
  static StateManager* Instance() {
    if( _instance == 0 ) {
      _instance = new StateManager;
    }
    return _instance;
  }

  // Game State DoInput Function
  void DoInput();

  // Game State DoUpdate Function
  void DoUpdate();

  // Game State DoRender Function
  void DoRender();

  // Game State OnLostDevice Function
  void OnLostDevice();

  // Game State OnResetDevice Function
  void OnResetDevice();

  // Setting the current state of the game
  void SetCurrentState( String state_name );

  // Insert a known state
  void InsertState( String state_name, State* state_pointer );

private:
  // Game State Manager instance
  static StateManager* _instance;
  // Make the state class the game_state_manager's friend
  friend class State;
  // Store the current state
  State* _current_state;
  // Map holding all states known
  std::map< String, State* > known_states;
  // Map iterator of known states
  std::map< String, State* >::iterator known_states_iterator; 
  
  // Game State Constructor
  StateManager();
	void operator delete(void*) {}; // such that its not deleted accidentally
};

#endif // __STATE_MANAGER
