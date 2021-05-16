#include "StateManager.h"

// Set the game state
StateManager* StateManager::_instance = 0;

// Game State Constructor
StateManager::StateManager() {
  // Set the current state to be nothing
  _current_state = NULL;
}

// Game State Deconstructor
StateManager::~StateManager() {
  // Release all the states we know about
  // Loop through all states
  while( known_states.size() > 0 ) {
    // Set the effect position to the beginning
    known_states_iterator = known_states.begin();
    // Call the effect_class release function
    known_states_iterator->second->~State();
    // Erase the effect from the map
    known_states.erase( known_states_iterator );
  }

  // Be sure to have the current state set to nothing
  _current_state = NULL;
}

// State DoInput Function
void StateManager::DoInput() {
  // Call the current state's DoInput Function
  _current_state->DoInput();
}

// State DoUpdate Function
void StateManager::DoUpdate() {
  // Call the current state's DoUpdate Function
  _current_state->DoUpdate();
}

// State DoRender Function
void StateManager::DoRender() {
  // Call the current state's DoRender Function
  _current_state->DoRender();
}

// State OnLostDevice Function
void StateManager::OnLostDevice() {
  // Call the current state's OnLostDevice Function
  _current_state->OnLostDevice();
}

// State OnResetDevice Function
void StateManager::OnResetDevice() {
  // Call the current state's OnResetDevice Function
  _current_state->OnResetDevice();
}

// Set the current program state
void StateManager::SetCurrentState( String state_name ) {
  // Check to see if the state exists, if it does, then set it to the current state
  known_states_iterator = known_states.begin();
  known_states_iterator = known_states.find( state_name );

  // Check for state
  if( known_states_iterator == known_states.end() ) {
    // State was not found
  } else {
    // Tell the state to unload certain items during state change
    if( _current_state != NULL ) {
      _current_state->OnLeaveState();
    }
    // Set the state to the found instance
    _current_state = known_states_iterator->second;
    // Tell the new state to load certain items during state change
    _current_state->OnLoadState();
  }
}

void StateManager::InsertState( String state_name, State* state_pointer ) {
  // Insert a new state into the map for use in set current state
  known_states.insert( std::make_pair( state_name, state_pointer ) );
}
