#include <Wire.h>                       // I2C
#include <WiFi.h>                       // Wifi driver
#include <WiFiClient.h>                 // Wireless Client
#include <WiFiAP.h>                     // Wireless Access Point
#include <Adafruit_GFX.h>               // Graphics
#include "Adafruit_ThinkInk.h"          // E-Paper Display Driver
#include <Adafruit_ImageReader_EPD.h>   // E-Paper Display Image-reading
#include <SdFat.h>                      // SD card & FAT filesystem

// Fonts for display
#include "fonts/Sail_Regular22pt7b.h"
#include "fonts/JosefinSans_VariableFont_wght7pt7b.h"
#include "fonts/JosefinSans_VariableFont_wght8pt7b.h"
#include "fonts/JosefinSans_VariableFont_wght12pt7b.h"
#include "fonts/JosefinSans_VariableFont_wght22pt7b.h"
#include "fonts/JosefinSans_VariableFont_wght30pt7b.h"
#include "fonts/JosefinSans_VariableFont_wght60pt7b.h"

// Set our pin details
#define SD_CS       33  // SD card chip select
#define EPD_DC      3   // E-Paper data command pin, can be any pin, but required for controlling the display
#define EPD_CS      1   // E-Ink chip celect, can be any pin, but required for controlling the display
#define EPD_BUSY    -1  // can set to -1 to not use a pin (will wait a fixed delay)
#define SRAM_CS     38  // S-Ram chip select, can set to -1 to not use a pin (uses a lot of RAM!)
#define EPD_RESET   21  // can set to -1 and share with chip Reset (can't deep sleep) - Uses LDO 2 EN
#define LIPO_ALERT  5   // Alert pin for MAX17043 Lipo monitor

// Set I2C Devices
#define MAX17043_ADDRESS 0x36

// Define button pins
#define Button1  18 // Add
#define Button2  14 // Function
#define Button3  12 // Minus

// Wifi details
#define WIFI_SSID "lifecounter-ap"
#define WIFI_PSK  "password"
#define WIFI_OFF_TIME 5000 //60000
IPAddress local_IP(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 2);
IPAddress subnet(255, 255, 255, 0);
IPAddress IP;

// Web Server
WiFiServer server(80);
String header; // Variable to store the HTTP request
bool wifiOn = true;

// SD Card
SdFat SD;         // SD card filesystem
Adafruit_ImageReader_EPD reader(SD); // Image-reader object, pass in SD filesys

// E-Ink Screen Setup
ThinkInk_290_Grayscale4_T5 display( EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY ); // Define the E-Paper display details

// Define Images
// Splash Screens
#define START_SPLASH "/splash-screens/MagicStartSplash.bmp"
#define DEATH_SPLASH "/splash-screens/YouDied.bmp"

// Mana Symbols
#define MC_0 "/mana-symbols/0.bmp"
#define MC_1 "/mana-symbols/1.bmp"
#define MC_2 "/mana-symbols/2.bmp"
#define MC_3 "/mana-symbols/3.bmp"
#define MC_4 "/mana-symbols/4.bmp"
#define MC_5 "/mana-symbols/5.bmp"
#define MC_6 "/mana-symbols/6.bmp"
#define MC_7 "/mana-symbols/7.bmp"
#define MC_8 "/mana-symbols/8.bmp"
#define MC_9 "/mana-symbols/9.bmp"
#define MC_10 "/mana-symbols/10.bmp"
#define MC_X "/mana-symbols/X.bmp"
#define MC_U "/mana-symbols/blue.bmp"
#define MC_G "/mana-symbols/green.bmp"
#define MC_R "/mana-symbols/red.bmp"
#define MC_B "/mana-symbols/black.bmp"
#define MC_W "/mana-symbols/white.bmp"
#define MC_P "/mana-symbols/phyrexian.bmp"
#define MC_S "/mana-symbols/snow.bmp"
#define MC_BG "/mana-symbols/blue-green.bmp"
#define MC_BR "/mana-symbols/black-red.bmp"
#define MC_UB "/mana-symbols/blue-black.bmp"
#define MC_UR "/mana-symbols/blue-red.bmp"
#define MC_C "/mana-symbols/colourless.bmp"
#define MC_GU "/mana-symbols/green-blue.bmp"
#define MC_GW "/mana-symbols/green-white.bmp"
#define MC_RG "/mana-symbols/red-green.bmp"
#define MC_RW "/mana-symbols/red-white.bmp"
#define MC_WB "/mana-symbols/white-black.bmp"
#define MC_WU "/mana-symbols/white-blue.bmp"

// Define configuration files/locations
#define COMMANDERS "/commander-setups/"
#define PREFERENCES_FILE "/preferences.txt"

// Define overall application status items
bool setupComplete = false;
bool hasBeenWoken = false;
unsigned long startupCompleteTime = 0;
unsigned long timeSinceStartup = 0;

// Lipo variables
float batVoltage;
float batPercentage;
int alertStatus;
bool showVoltage = false;
bool showAmpers = false;
bool showPercentage = false;

// Application interaction variables
bool lifeTotalUpdate = false;
int lifeTotal = 0;

// Gametype struct
struct gameFormats {
  int Commander = 1;
  int Standard = 2;
  int Modern = 3;
  int Draft = 4;
} gameTypes;

int gameType = 0;

void setup() {
  // Create the serial connection within the first 5seconds, if not we continue as its probably not connected to a computer
  Serial.begin( 115200 );
  int serialAttempts = 0;
  while( !Serial && serialAttempts < 500 ) {
    delay( 10 );
    serialAttempts++;
  }
  
  // Create wireless AP
  WiFi.softAPConfig(local_IP, gateway, subnet);
  Serial.print( "Setting AP (Access Point)…" );
  WiFi.softAP( WIFI_SSID, WIFI_PSK );

  IP = WiFi.softAPIP();
  Serial.print("Access Point IP address: ");
  Serial.println( IP );

  // Start working on our display
  display.begin();

  display.clearBuffer();
  display.setTextSize(2);
  display.setTextColor(EPD_BLACK);
// Make fancier with draw calls
  display.setCursor(15,15);
  display.println("Life Counter v0.2");
  display.setCursor(15,32);
  display.println("By: Bozza");
  display.setCursor(15,49);
  display.println("www.bozau.cloud");
  display.setCursor(15,70);

  display.setTextSize(1);
  display.println("Initialzing...");
  
  // SD card is pretty straightforward, a single call...
  Serial.print("Initializing filesystem...");
  if(!SD.begin(SD_CS, SD_SCK_MHZ(10))) { // Breakouts require 10 MHz limit due to longer wires
    Serial.println(F("SD begin() failed"));
    display.println("File System Status: FAILED");
    display.println("Please check SD Card");
    display.display();
    delay( 5000 );
    for(;;); // Fatal error, do not continue
  } else {
    Serial.println("OK!");
    display.setCursor(15,84);
    display.println("File System OK");
    display.setCursor(15,92);
    display.println("I2C");
    display.setCursor(15,100);
    display.println("Input");
    display.setCursor(15,108);
    display.println("Wifi");
    display.setCursor(15,116);
    display.println("Battery Monitor");
  }
  
  Serial.println("Starting I2C.");
  
  Wire.begin();         // Start I2C
  configMAX17043(35);   // Configure the MAX17043's alert percentage
  qsMAX17043();         // restart fuel-gauge calculations

  Serial.println("Setting up input pins.");
  
  // Setup user input pins
  pinMode(Button1, INPUT);
  pinMode(Button2, INPUT);
  pinMode(Button3, INPUT);

  // Setup Lipo battery alert pin
  pinMode(LIPO_ALERT, INPUT_PULLUP);

  // Start the Wifi connection
  server.begin();
  Serial.println("Wireless Access Point online.");
  display.display();
  delay( 5000 );
    
  renderSplash();
  delay( 5000 ); // Protect the screen

  Serial.println("System setup complete.");
  startupCompleteTime = millis(); // Get time that we finished starting up the system
}

void loop() {
  // For each loop, calculate the time passed since startup completed, useful for 'state' machines
  timeSinceStartup = timePassed(); // TODO

  // Validate if we've woken in this loop
  checkIfHasBeenWoken();

  // Turn off the wifi after WIFI_OFF_TIME (should be enough time to configure) and save battery
  if( ( timeSinceStartup > WIFI_OFF_TIME && wifiOn ) || setupComplete ) {
    if( wifiOn ) {
      while( WiFi.softAPdisconnect(true) ) {
        Serial.println("Turning WiFi off.");
        delay( 50 );
      }
      wifiOn = false;
      Serial.println("Wifi has been turned off.");
    }

    // Enter light sleep after no choice has been made and wifi's off
    if( !setupComplete ) {
      renderSplash();
      delay( 100 );
      while( 0 != putToLightSleep() ) {
        checkIfHasBeenWoken();
      }
    }
  }

  // Check if setup has been completed and run through to the life total state
  if( digitalRead( Button1 ) == HIGH && !setupComplete ) {
    setupComplete = true;
    gameType = gameTypes.Draft; // TODO - Implement chosen types on start rather than static
    lifeTotal = 20; // TOOD - Set correct lifetotal for chosen game type
    Serial.print("Life Total: ");
    Serial.println(lifeTotal);
    renderLife();
  }

  // If setup's compelete, then 
  if( setupComplete ) {
    if( digitalRead( Button1 ) == HIGH && digitalRead( Button2 ) == HIGH ) { // Death
      renderDeath();
      setupComplete = false; // Allow for reset
      Serial.println("Death");
      delay( 5000 ); // Protect the screen
    } else if( digitalRead( Button1 ) == HIGH ) { // Increase life total by 1 each time - TODO, allow multiple presses before screen re-render
      lifeTotal++;
      Serial.print("Life Total: ");
      Serial.println(lifeTotal);
      lifeTotalUpdate = true;
      Serial.println("Increase Life");
    } else if( digitalRead( Button3 ) == HIGH ) { // Decrease life total by 1 each time - TODO, allow multiple presses before screen re-render
      lifeTotal--;
      Serial.print("Life Total: ");
      Serial.println(lifeTotal);
      Serial.println("Reduce Life");
      lifeTotalUpdate = true;
    }

    if( lifeTotalUpdate ) { // TODO
      lifeTotalUpdate = false;
      renderLife();
    }
  }
}

unsigned long hasPastEndTime( unsigned long startTime, unsigned long endTime ) {
  return timePassed(); // TO DO / Think about helping with sleep
}

unsigned long timePassed() {
  return millis() - startupCompleteTime;
}

void renderSplash() {
  ImageReturnCode stat; // Status from image-reading functions
  // Render our splash screen
  display.clearBuffer();
  
  stat = reader.drawBMP(START_SPLASH, display, 0, 0);
  reader.printStatus(stat); // How'd we do?  
  
  display.setTextColor(EPD_WHITE);
  if( wifiOn ) { // Don't render Wifi details if its turned off
    display.setCursor(10,95);
    //display.setTextSize(1);

    display.print("WIFI: ");
    display.print(WIFI_SSID);
    display.print(" - IP: ");
    display.print(IP);
  }
  //display.setTextSize(2);

  display.setCursor(20,107);
  display.print("Commander");
  display.fillTriangle(135, 111, 145, 111, 140, 121, EPD_WHITE);
  display.setCursor(175,107);
  display.print("Modern");
  display.fillTriangle(255, 111, 265, 111, 260, 121, EPD_WHITE);

  renderBatteryStatus();
  
  display.display();
}

void renderDeath() {
  ImageReturnCode stat; // Status from image-reading functions
  // Render our splash screen
  display.clearBuffer();
  
  stat = reader.drawBMP(DEATH_SPLASH, display, 0, 0);
  reader.printStatus(stat); // How'd we do?  

  display.display();
}

void renderLife() {
  Serial.print("Life Total: ");
  Serial.println(lifeTotal);

  display.clearBuffer();

  //renderGrid(); // Used for locating where the object on the screen is

  // Title
  display.setFont(&Sail_Regular22pt7b); // Requires a 3px buffer on Y
  display.setTextColor(EPD_BLACK);
  if( gameType == gameTypes.Commander ) {
    display.setCursor(42,26 + 2);
    display.print("Commander");
  } else if( gameType == gameTypes.Modern ) {
    display.setCursor(62,26 + 2);
    display.print("Modern");
  } else if( gameType == gameTypes.Standard ) {
    display.setCursor(50,26 + 2);
    display.print("Standard");
  } else if( gameType == gameTypes.Draft ) {
    display.setCursor(90,26 + 2);
    display.print("Draft");
  }

  // Life Value
  display.setTextColor(EPD_BLACK);
  if( gameType == gameTypes.Commander ) {
    display.setFont(&JosefinSans_VariableFont_wght60pt7b); // Requires a 22px buffer on Y and buffer on X
    display.setCursor(85,117);
    display.print(lifeTotal);
  } else {
    display.setFont(&JosefinSans_VariableFont_wght60pt7b); // Requires a 22px buffer on Y and buffer on X
    display.setCursor(85,117);
    display.print(lifeTotal);
  }

  renderBatteryStatus();
  
  display.display();
  delay( 5000 );
}


void renderBatteryStatus( ) {
  ImageReturnCode stat; // Status from image-reading functions

  updateBatteryStats();
  
  display.setFont(&JosefinSans_VariableFont_wght7pt7b);
  display.setTextColor(EPD_DARK);
  display.setCursor(245,10);
  if( batPercentage >= 100 ) {
    display.print( "100%" );
  } else {
    display.print( batPercentage, 0 );
    display.println( "%" );
  }

  // Render battery icon
  int batteryPosX = 276;
  int batterySegmentPosX = 278;
  int batterySegmentPosY = 3;
  int batterySegmentIncRate = 4;
  int segmentsToRender = 0;

  // Outline
  display.drawRect( 276, 1, 19, 10, EPD_DARK);
  display.drawLine( 275, 3, 275, 9, EPD_DARK);
  
  // Segments
  if( batPercentage >= 80.0f ) {
    segmentsToRender = 4;
  } else if( batPercentage >= 60.0f ) {
    segmentsToRender = 3;
  } else if( batPercentage >= 40.0f ) {
    segmentsToRender = 2;
  } else if( batPercentage >= 40.0f ) {
    segmentsToRender = 1;
  }

  // Render left to right
  for( int i = 0; i < segmentsToRender; i++ ) {
    display.drawRect( batterySegmentPosX, batterySegmentPosY, 3, 6, EPD_DARK);
    display.drawLine( batterySegmentPosX+1, batterySegmentPosY+1, batterySegmentPosX+1, batterySegmentPosY+4, EPD_LIGHT);
    batterySegmentPosX += batterySegmentIncRate;
  }
}

void renderGrid() {
  // Render vertical lines every 20px
  for( int x = 19; x < 296; x = x + 20 ) {
    display.drawLine( x, 0, x, 128, EPD_LIGHT );
  }

  // Render horizontal lines every 20px // TODO

  // Render centre of EPD screen
  display.drawLine( 296/2, 0, 296/2, 128, EPD_DARK );
  display.drawLine( 0, 128/2, 296, 128/2, EPD_DARK );
}

int putToLightSleep() {
    Serial.println( "Enabling Light Sleep; " );
    enableWakeSource();
    hasBeenWoken = true; // Set to true as this will mean we've been worken up next time
    return esp_light_sleep_start();
}
void checkIfHasBeenWoken() {
  if( hasBeenWoken ) {
    print_wakeup_reason();
    hasBeenWoken = false;
  }
}

// Function to enable wakeup sources from deep sleep
void enableWakeSource() {
  Serial.print("Enabling GPIO Wake GPIO 18, 14, 12: ");
  Serial.print(gpio_wakeup_enable( GPIO_NUM_18, GPIO_INTR_HIGH_LEVEL )); // Button1
  Serial.print( digitalRead( 18 ) );
  Serial.print(", ");
  Serial.print(gpio_wakeup_enable( GPIO_NUM_14, GPIO_INTR_HIGH_LEVEL )); // Button2
  Serial.print( digitalRead( 14 ) );
  Serial.print(", ");
  Serial.print( gpio_wakeup_enable( GPIO_NUM_12, GPIO_INTR_HIGH_LEVEL ) ); // Button3
  Serial.println( digitalRead( 12 ) );

  Serial.print("Enabling GPIO Wake: ");
  Serial.println( esp_sleep_enable_gpio_wakeup() );
}

void disableWakeSource() {
  // TODO
}

// Function to find out wake reason
void print_wakeup_reason(){
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

// Function to invoke updating the battery status information
void updateBatteryStats() {
  
  // Get our battery status details
  batPercentage = percentMAX17043();
  batVoltage = (float) vcellMAX17043() * 1/800;  // vcell reports battery in 1.25mV increments
  alertStatus = digitalRead(LIPO_ALERT);

  Serial.print(batPercentage, 2);  // Print the battery percentage
  Serial.println(" %");
  Serial.print(batVoltage, 2);  // print battery voltage
  Serial.println(" V");
  Serial.print("Alert Status = ");
  Serial.println(alertStatus, DEC);
  Serial.println();
  
}

// MAX17043 - LiPo Battery Monitor helper functions
/*
vcellMAX17043() returns a 12-bit ADC reading of the battery voltage,
as reported by the MAX17043's VCELL register.
This does not return a voltage value. To convert this to a voltage,
multiply by 5 and divide by 4096.
*/
unsigned int vcellMAX17043()
{
  unsigned int vcell;

  vcell = i2cRead16(0x02);
  vcell = vcell >> 4;  // last 4 bits of vcell are nothing

  return vcell;
}

/*
percentMAX17043() returns a float value of the battery percentage
reported from the SOC register of the MAX17043.
*/
float percentMAX17043()
{
  unsigned int soc;
  float percent;

  soc = i2cRead16(0x04);  // Read SOC register of MAX17043
  percent = (byte) (soc >> 8);  // High byte of SOC is percentage
  percent += ((float)((byte)soc))/256;  // Low byte is 1/256%

  return percent;
}

/* 
configMAX17043(byte percent) configures the config register of
the MAX170143, specifically the alert threshold therein. Pass a 
value between 1 and 32 to set the alert threshold to a value between
1 and 32%. Any other values will set the threshold to 32%.
*/
void configMAX17043(byte percent)
{
  if ((percent >= 32)||(percent == 0))  // Anything 32 or greater will set to 32%
    i2cWrite16(0x9700, 0x0C);
  else
  {
    byte percentBits = 32 - percent;
    i2cWrite16((0x9700 | percentBits), 0x0C);
  }
}

/* 
qsMAX17043() issues a quick-start command to the MAX17043.
A quick start allows the MAX17043 to restart fuel-gauge calculations
in the same manner as initial power-up of the IC. If an application's
power-up sequence is very noisy, such that excess error is introduced
into the IC's first guess of SOC, the Arduino can issue a quick-start
to reduce the error.
*/
void qsMAX17043()
{
  i2cWrite16(0x4000, 0x06);  // Write a 0x4000 to the MODE register
}

/* 
i2cRead16(unsigned char address) reads a 16-bit value beginning
at the 8-bit address, and continuing to the next address. A 16-bit
value is returned.
*/
unsigned int i2cRead16(unsigned char address)
{
  int data = 0;

  Wire.beginTransmission(MAX17043_ADDRESS);
  Wire.write(address);
  Wire.endTransmission();

  Wire.requestFrom(MAX17043_ADDRESS, 2);
  while (Wire.available() < 2)
    ;
  data = ((int) Wire.read()) << 8;
  data |= Wire.read();

  return data;
}

/*
i2cWrite16(unsigned int data, unsigned char address) writes 16 bits
of data beginning at an 8-bit address, and continuing to the next.
*/
void i2cWrite16(unsigned int data, unsigned char address)
{
  Wire.beginTransmission(MAX17043_ADDRESS);
  Wire.write(address);
  Wire.write((byte)((data >> 8) & 0x00FF));
  Wire.write((byte)(data & 0x00FF));
  Wire.endTransmission();
}
