
/*
 * Library for a simple debouncing button
 */

#ifndef Button_h
#define button_h

#include "Arduino.h"

class Button
{
  public:
    Button(int pin);
    boolean pressed();

  private:
    int _pin;
    int _lastButtonState;
    int _buttonState;
    long _lastDebounceTime;
    long _debounceDelay;
};
#endif
