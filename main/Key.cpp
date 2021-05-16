#include <Arduino.h>
#include "Key.h"

Key::Key( int _keyPin, uint8_t _keyMode, bool _modKey ) {
  Key( _keyPin, _keyMode, GPIO_NUM_1, NULL, NULL, _modKey );
}
  
Key::Key( int _keyPin, uint8_t _keyMode = INPUT, gpio_num_t _keyGpioPin = GPIO_NUM_1, bool _gpioSleepEnabled = false, bool _gpioSleepHigh = false, bool _modKey = false ) {
  keyPin = _keyPin;
  keyMode = _keyMode;
  keyGpioPin = _keyGpioPin;
  gpioSleepEnabled = _gpioSleepEnabled;
  gpioSleepHigh = _gpioSleepHigh;
  modKey = _modKey;

  lastKeyState = LOW;

  Key::SetupKey();
}

void Key::SetupKey() {
  pinMode(keyPin, keyMode);
  EnableGPIOWake();
}

int Key::EnableGPIOWake() {
  if( gpioSleepEnabled ) {
      if( gpioSleepHigh ) {
        gpio_wakeup_enable( keyGpioPin, GPIO_INTR_HIGH_LEVEL );
      } else {
        gpio_wakeup_enable( keyGpioPin, GPIO_INTR_LOW_LEVEL );
      }
      return esp_sleep_enable_gpio_wakeup();
  }
  return 0;
}

int Key::EnableGPIOWake( gpio_num_t _keyGpioPin ) {
  keyGpioPin = _keyGpioPin;
  gpioSleepEnabled = true;
  return EnableGPIOWake();
}

void Key::EnableModKey() {
  modKey = true;
}
void Key::DisableModKey() {
  modKey = false;
}
bool Key::IsModKey() {
  return modKey;
}
