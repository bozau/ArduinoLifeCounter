#ifndef __KEY
#define __KEY

#include <Arduino.h>

// Setup a key class to help manage our button presses in a more controlled manner
class Key {
  public:
    Key();
    Key( int _keyPin, uint8_t _keyMode, bool _modKey );
    Key( int _keyPin, uint8_t _keyMode, gpio_num_t _keyGpioPin, bool _gpioSleepEnabled, bool _gpioSleepHigh, bool _modKey );

    int EnableGPIOWake();
    int EnableGPIOWake( gpio_num_t _keyGpioPin );
    
    int EnableRTCWake();
    int EnableRTCWake( gpio_num_t _keyGpioPin );

    void EnableModKey();
    void DisableModKey();
    bool IsModKey();
    
  private:
    int lastKeyState;
    int keyPin;
    gpio_num_t keyGpioPin;
    uint8_t keyMode;
    bool gpioSleepEnabled;
    bool gpioSleepHigh;
    bool modKey; // Key is a modifier type key
    int modTime; // How long its pressed down to enable modifier status in milliseconds
    unsigned long start; // When the key was pressed
    unsigned long end; // When the key was released

    void SetupKey();

};

#endif
