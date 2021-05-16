#ifndef __LIPO
#define __LIPO

#include <Arduino.h>
#include <Wire.h>                       // I2C
#include "Structs.h"
#include "ScreenManager.h"

class LiPo {
    public:
	LiPo(LiPo&) = delete; // Copy prohibited
    void operator=(const LiPo&) = delete; // Assignment prohibited
    LiPo& operator=(LiPo&&) = delete; // Move assignment


        // Game State Deconstructor
        ~LiPo();

        // Game State Instance Function
        static LiPo* Instance() {
            if( _instance == 0 ) {
              _instance = new LiPo();
            }
            return _instance;
        }

        void GetBatteryStats( BatteryStatus* stat );
        void SetAlertPin( int pin );
        void SetI2CAddress( unsigned char address );
        void RenderBatteryStatus( BatteryStatus* stat );

    private:
        // Game State Manager instance
        static LiPo* _instance;
        int alertPin;
        unsigned char MAX17043_ADDRESS;

        // Game State Constructor
        LiPo();
	void operator delete(void*) {}; // such that its not deleted accidentally
        unsigned int i2cRead16(unsigned char address);
        void i2cWrite16(unsigned int data, unsigned char address);
        void qsMAX17043();
        float vcellMAX17043();
        float percentMAX17043();
        void configMAX17043(byte percent);
};

#endif // __LIPO
