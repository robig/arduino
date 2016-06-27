
/*
 * Library for a state switching button
 */

#ifndef Switch_h
#define Switch_h

#include "Arduino.h"

class Switch
{
  public:
    Switch(int pin);
    void process();

    int getState();
    boolean isHIGH();
    boolean stateChanged();

  private:
    int _pin;
    long _release;
    int _state;
    int _value;
    int _lastValue;
    boolean _statechange;
    unsigned long _previousMillis;
};
#endif
