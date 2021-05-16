#ifndef __SCREEN_MANAGER
#define __SCREEN_MANAGER

#include <Arduino.h>
#include <Adafruit_GFX.h>               // Graphics
#include "Adafruit_ThinkInk.h"          // E-Paper Display Driver
#include <Adafruit_ImageReader_EPD.h>   // E-Paper Display Image-reading
#include "Fonts.h"
#include "HelperFunctions.h"

#define EPD_DC      3   // E-Paper data command pin, can be any pin, but required for controlling the display
#define EPD_CS      1   // E-Ink chip celect, can be any pin, but required for controlling the display
#define EPD_BUSY    -1  // can set to -1 to not use a pin (will wait a fixed delay)
#define SRAM_CS     38  // S-Ram chip select, can set to -1 to not use a pin (uses a lot of RAM!)
#define EPD_RESET   21  // can set to -1 and share with chip Reset (can't deep sleep) - Uses LDO 2 EN

class ScreenManager {

public:
	ScreenManager(ScreenManager&) = delete; // Copy prohibited
    void operator=(const ScreenManager&) = delete; // Assignment prohibited
    ScreenManager& operator=(ScreenManager&&) = delete; // Move assignment

  // Game State Deconstructor
  ~ScreenManager();

  // Game State Instance Function
  static ScreenManager* Instance() {
    if( _instance == 0 ) {
      _instance = new ScreenManager();
    }
    return _instance;
  }

  void ClearScreen();
  void ScreenBufferReady();
  void UpdateScreen();
  bool IsBufferReady();
  ThinkInk_290_Grayscale4_T5 GetScreen();

private:
  // Game State Manager instance
  static ScreenManager* _instance;
  unsigned long timeSinceLastScreenUpdate;
  unsigned long waitTimeTillUpdate; // Minimum wait time till we can refresh the screen
  //ThinkInk_290_Grayscale4_T5 display;
  bool screenBufferInvalidated;

  ScreenManager();
	void operator delete(void*) {}; // such that its not deleted accidentally
};

#endif // __SCREEN_MANAGER
