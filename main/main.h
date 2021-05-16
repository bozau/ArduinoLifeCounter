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

// SD Card
SdFat SD;         // SD card filesystem
Adafruit_ImageReader_EPD reader(SD); // Image-reader object, pass in SD filesys

// Define configuration files/locations
#define COMMANDERS "/commander-setups/"
#define PREFERENCES_FILE "/preferences.txt"

// Define overall application status items
bool setupComplete = false;
bool hasBeenWoken = false;
bool powerLed = true;

extern unsigned long startupCompleteTime = 0;
unsigned long timeSinceStartup = 0;
unsigned long timeOfLastAction = 0;
unsigned long timeToDeepSleep = 10 * 60 * 1000; // Number of minutes till we should enter deep sleep (mins, seconds, milliseconds)
unsigned long timeToWaitAfterLastAction = 1.5 * 1000;

// Create our list of joystick location ranges;
std::vector< JoystickZone > JoystickZones;

// Key configurations
std::vector< Key* > Keys;

// Battery Stats Struct
BatteryStatus batteryStats;
#endif
