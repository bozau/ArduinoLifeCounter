#include <Arduino.h>
#include <Wire.h>                       // I2C
#include <SdFat.h>                      // SD card & FAT filesystem
#include <vector>

#include "Structs.h"

#include "ScreenManager.h"
#include "WifiManager.h"
#include "JoystickManager.h"
#include "StateManager.h"

#include "HelperFunctions.h"
#include "Key.h"
#include "Images.h"
#include "LiPo.h"
#include "GameState.h"

// State Includes
#include "SplashState.h"
#include "DeathState.h"
#include "CommonState.h"
#include "CommanderState.h"

#ifndef __MAIN
#define __MAIN

// Set our pin details
#define SD_CS       33  // SD card chip select
#define POWER_BUTTON 6  // Power button pin
#define POWER_LED    11 // Power led on button pin
#define POWER_LED_GPIO GPIO_NUM_11

// Define configuration files/locations
#define COMMANDERS "/commander-setups/"
#define PREFERENCES_FILE "/preferences.txt"

// Define overall application status items
extern bool setupComplete;// = false;
extern bool hasBeenWoken;// = false;
extern bool powerLed;// = true;

extern unsigned long startupCompleteTime;// = 0;
extern unsigned long timeSinceStartup;// = 0;
extern unsigned long timeOfLastAction;// = 0;
extern unsigned long timeToDeepSleep;// = 10 * 60 * 1000; // Number of minutes till we should enter deep sleep (mins, seconds, milliseconds)
extern unsigned long timeToWaitAfterLastAction;// = 1.5 * 1000;

#endif
