#include "main.h"

// Define overall application status items
bool setupComplete = false;
bool hasBeenWoken = false;
bool powerLed = true;

unsigned long startupCompleteTime = 0;
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

// SD Card
SdFat SD;         // SD card filesystem
Adafruit_ImageReader_EPD reader(SD); // Image-reader object, pass in SD filesys

void setup() {
  // Turn on power led
  pinMode( POWER_LED, OUTPUT );
  UpdatePowerLed( true );

  // Power button
  pinMode( POWER_BUTTON, INPUT );

  // Setup key inputs
  Keys.push_back( new Key( 18, INPUT, GPIO_NUM_18, true, true, false ) );
  Keys.push_back( new Key( 14, INPUT, GPIO_NUM_14, true, true, false ) );
  Keys.push_back( new Key( 12, INPUT, GPIO_NUM_12, true, true, false ) );

  // Create the serial connection within the first 10 seconds if all three buttons are pressed, if not we continue as its probably not connected to a computer
  Serial.begin( 115200 );

  //if( digitalRead( Button1 ) == HIGH && digitalRead( Button2 ) == HIGH && digitalRead( Button3 ) == HIGH ) {
    int serialAttempts = 0;
    while( !Serial && serialAttempts < 1000 ) {
      delay( 10 );
      serialAttempts++;
    }
  //}
  
  // Start I2C
  Serial.println( "Starting I2C." );
  Wire.begin();

  // Setup States
  StateManager::Instance()->InsertState( "splash_state", SplashScreenState::Instance() );
  StateManager::Instance()->InsertState( "death_state", DeathScreenState::Instance() );
  StateManager::Instance()->InsertState( "common_state", CommonScreenState::Instance() );
  StateManager::Instance()->InsertState( "commander_state", CommanderScreenState::Instance() );

  StateManager::Instance()->SetCurrentState( "splash_state" );

  // Setup LiPo Monitor
  LiPo::Instance()->SetAlertPin( 5 );
  LiPo::Instance()->SetI2CAddress( 0x36 );
  LiPo::Instance()->GetBatteryStats( &batteryStats );

  Serial.print(batteryStats.percentage, 2);  // Print the battery percentage
  Serial.println(" %");
  Serial.print(batteryStats.voltage, 2);  // print battery voltage
  Serial.println(" V");
  Serial.print("Alert Status = ");
  Serial.println(batteryStats.alertStatus, DEC);
  Serial.println();


  // SD card is pretty straightforward, a single call...
  Serial.print("Initializing filesystem...");
  if(!SD.begin(SD_CS, SD_SCK_MHZ(10))) { // Breakouts require 10 MHz limit due to longer wires
    ScreenManager::Instance()->ClearScreen();
    ScreenManager::Instance()->GetScreen().setTextSize(2);
    ScreenManager::Instance()->GetScreen().setTextColor(EPD_BLACK);
    Serial.println(F("SD begin() failed"));
    ScreenManager::Instance()->GetScreen().println("File System Status: FAILED");
    ScreenManager::Instance()->GetScreen().println("Please check SD Card");
    ScreenManager::Instance()->GetScreen().display();
    delay( 5000 );
    for(;;); // Fatal error, do not continue
  }

  // Setup joystick locations (can also consider reading this from SD Card)
  //JoystickZones.push_back( JoystickZone{ 3500, 0, 6100, 4100, JoystickLocations.Left } );

  Serial.println("System setup complete.");
  startupCompleteTime = millis(); // Get time that we finished starting up the system
  timeSinceStartup = TimePassed();
}

void loop() {
  // For each loop, calculate the time passed since startup completed, useful for 'state' machines
  timeSinceStartup = TimePassed(); // TODO

  // Validate if we've woken in this loop
  CheckIfHasBeenWoken();

  // Run Current State actions
  StateManager::Instance()->DoInput();
  StateManager::Instance()->DoUpdate();
  StateManager::Instance()->DoRender(); // Creates rendering of the screen into the 'screen buffer' if required

  // Render battery status only if screen buffer is ready for render
  if( ScreenManager::Instance()->IsBufferReady() ) {
    LiPo::Instance()->RenderBatteryStatus( &batteryStats );
  }

  // Updates the ScreenManager::Instance()->GetScreen() if the 'screen buffer' has been updated and passed a set quantity of time
  ScreenManager::Instance()->UpdateScreen(); 
}

// Application utility functions to support main loop
void CheckIfHasBeenWoken() {
  if( hasBeenWoken ) {
    GetWakeupReason();
    hasBeenWoken = false;
    UpdatePowerLed( true );
  }
}

// Function to find out wake reason
void GetWakeupReason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    case ESP_SLEEP_WAKEUP_GPIO : Serial.println("Wakeup caused by GPIO"); break;
    case ESP_SLEEP_WAKEUP_UART : Serial.println("Wakeup caused by UART"); break;
    case ESP_SLEEP_WAKEUP_WIFI : Serial.println("Wakeup caused by WIFI"); break;
    case ESP_SLEEP_WAKEUP_COCPU : Serial.println("Wakeup caused by Co CPU"); break;
    case ESP_SLEEP_WAKEUP_COCPU_TRAP_TRIG : Serial.println("Wakeup caused by Co CPU trap trigger"); break;
    case ESP_SLEEP_WAKEUP_BT : Serial.println("Wakeup caused by BT"); break;
    default : Serial.printf("Wakeup was not caused by sleep: %d\n",wakeup_reason); break;
  }
}

// Power LED Control
void UpdatePowerLed( bool newLedStatus ) {
  powerLed = newLedStatus;
  if( powerLed ) {
    digitalWrite( POWER_LED, HIGH );
    gpio_hold_en( POWER_LED_GPIO );
  } else {
    digitalWrite( POWER_LED, LOW );
  }
}
