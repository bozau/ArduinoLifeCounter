#include "ScreenManager.h"

// Set the game state
ScreenManager* ScreenManager::_instance = 0;

ThinkInk_290_Grayscale4_T5 display( EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY );

// Game State Constructor
ScreenManager::ScreenManager() {
  // E-Ink Screen Setup

  display.begin();
  screenBufferInvalidated = true;
  waitTimeTillUpdate = 5000;
}

// Game State Deconstructor
ScreenManager::~ScreenManager() {
}

void ScreenManager::ClearScreen() {
    display.clearBuffer();
}

void ScreenManager::ScreenBufferReady() {
    screenBufferInvalidated = true;
}

bool ScreenManager::IsBufferReady() {
    return screenBufferInvalidated;
}

ThinkInk_290_Grayscale4_T5 ScreenManager::GetScreen() {
    return display;
}

// State DoRender Function
void ScreenManager::UpdateScreen() {
    if( HasPastEndTime( timeSinceLastScreenUpdate, waitTimeTillUpdate ) && screenBufferInvalidated ) {
        display.display();
        screenBufferInvalidated = false;
    }
}
