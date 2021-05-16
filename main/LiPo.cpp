#include "LiPo.h"

// Set the state
LiPo* LiPo::_instance = 0;

// Game State Constructor
LiPo::LiPo() {
    // Set the current state to be nothing
    _instance = NULL;
    alertPin = -1;
}

// Game State Deconstructor
LiPo::~LiPo() {
}
  
// Function to invoke updating the battery status information
void LiPo::GetBatteryStats( BatteryStatus* stat ) {
    // Get our battery status details
    stat->percentage = percentMAX17043();
    stat->voltage = vcellMAX17043(); // vcell reports battery in 1.25mV increments
    if( alertPin != -1 ) {
      stat->alertStatus = digitalRead( alertPin );
    }
}

void LiPo::SetAlertPin( int _alertPin ) {
  alertPin = _alertPin;
}

void LiPo::SetI2CAddress( unsigned char address ) {
  MAX17043_ADDRESS = address;
}

void LiPo::RenderBatteryStatus( BatteryStatus* stat ) {
    ScreenManager::Instance()->GetScreen().setFont( &JosefinSans_VariableFont_wght7pt7b );
    ScreenManager::Instance()->GetScreen().setTextColor(EPD_DARK);
    if( stat->percentage >= 100 ) {
        ScreenManager::Instance()->GetScreen().setCursor(245,10);
        ScreenManager::Instance()->GetScreen().print( "100%" );
    } else {
        ScreenManager::Instance()->GetScreen().setCursor(249,10);
        ScreenManager::Instance()->GetScreen().print( stat->percentage, 0 );
        ScreenManager::Instance()->GetScreen().println( "%" );
    }

    // Battery low
    if( stat->percentage <= 35.0f ) {
        ScreenManager::Instance()->GetScreen().setCursor(245,20);
        ScreenManager::Instance()->GetScreen().println( "BATTERY LOW" );
    }

    // Render battery icon
    int batteryPosX = 276;
    int batterySegmentPosX = 278;
    int batterySegmentPosY = 3;
    int batterySegmentIncRate = 4;
    int segmentsToRender = 0;

    // Outline
    ScreenManager::Instance()->GetScreen().drawRect( 276, 1, 19, 10, EPD_DARK);
    ScreenManager::Instance()->GetScreen().drawLine( 275, 3, 275, 9, EPD_DARK);

    // Segments
    int segmentQty = 4;
    if( stat->percentage >= 80.0f ) {
        segmentsToRender = 4;
    } else if( stat->percentage >= 60.0f ) {
        segmentsToRender = 3;
    } else if( stat->percentage >= 40.0f ) {
        segmentsToRender = 2;
    } else if( stat->percentage >= 20.0f ) {
        segmentsToRender = 1;
    }

    // Move the render battery start point across
    batterySegmentPosX = 278 - ( batterySegmentIncRate * ( segmentsToRender - segmentQty ) );

    // Render left to right
    for( int i = 0; i < segmentsToRender; i++ ) {
        ScreenManager::Instance()->GetScreen().drawRect( batterySegmentPosX, batterySegmentPosY, 3, 6, EPD_DARK);
        ScreenManager::Instance()->GetScreen().drawLine( batterySegmentPosX+1, batterySegmentPosY+1, batterySegmentPosX+1, batterySegmentPosY+4, EPD_LIGHT);
        batterySegmentPosX += batterySegmentIncRate;
    }
}
/* 
i2cRead16(unsigned char address) reads a 16-bit value beginning
at the 8-bit address, and continuing to the next address. A 16-bit
value is returned.
*/
unsigned int LiPo::i2cRead16(unsigned char address) {
    int data = 0;

    Wire.beginTransmission( MAX17043_ADDRESS );
    Wire.write(address);
    Wire.endTransmission();
    Wire.requestFrom(MAX17043_ADDRESS, 2);
    while (Wire.available() < 2);
    data = ((int) Wire.read()) << 8;
    data |= Wire.read();

    return data;
}

/*
i2cWrite16(unsigned int data, unsigned char address) writes 16 bits
of data beginning at an 8-bit address, and continuing to the next.
*/
void LiPo::i2cWrite16(unsigned int data, unsigned char address) {
    Wire.beginTransmission( MAX17043_ADDRESS );
    Wire.write(address);
    Wire.write((byte)((data >> 8) & 0x00FF));
    Wire.write((byte)(data & 0x00FF));
    Wire.endTransmission();
}

/* 
configMAX17043(byte percent) configures the config register of
the MAX170143, specifically the alert threshold therein. Pass a 
value between 1 and 32 to set the alert threshold to a value between
1 and 32%. Any other values will set the threshold to 32%.
*/
void LiPo::configMAX17043(byte percent) {
    if((percent >= 32)||(percent == 0)) { // Anything 32 or greater will set to 32%
        i2cWrite16(0x9700, 0x0C);
    } else {
        byte percentBits = 32 - percent;
        i2cWrite16((0x9700 | percentBits), 0x0C);
    }
}

/*
percentMAX17043() returns a float value of the battery percentage
reported from the SOC register of the MAX17043.
*/
float LiPo::percentMAX17043() {
    unsigned int soc;
    float percent;

    soc = i2cRead16(0x04);  // Read SOC register of MAX17043
    percent = (byte) (soc >> 8);  // High byte of SOC is percentage
    percent += ((float)((byte)soc))/256;  // Low byte is 1/256%

    return percent;
}

// MAX17043 - LiPo Battery Monitor helper functions
/*
vcellMAX17043() returns a 12-bit ADC reading of the battery voltage,
as reported by the MAX17043's VCELL register.
This does not return a voltage value. To convert this to a voltage,
multiply by 5 and divide by 4096.
*/
float LiPo::vcellMAX17043() {
    unsigned int vcell;

    vcell = i2cRead16(0x02);
    vcell = vcell >> 4;  // last 4 bits of vcell are nothing

    return (float) vcell * 1/800;
}

/* 
qsMAX17043() issues a quick-start command to the MAX17043.
A quick start allows the MAX17043 to restart fuel-gauge calculations
in the same manner as initial power-up of the IC. If an application's
power-up sequence is very noisy, such that excess error is introduced
into the IC's first guess of SOC, the Arduino can issue a quick-start
to reduce the error.
*/
void LiPo::qsMAX17043() {
    i2cWrite16(0x4000, 0x06);  // Write a 0x4000 to the MODE register
}
