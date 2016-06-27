
/*
 * Library for an blinking led
 */

#ifndef Blinker_h
#define Blinker_h

#include "Arduino.h"

class Blinker
{
  public:
    Blinker(int pin);
    void process();

    void setInterval(long iv);
    long getInterval();
    
    void setEnabled(boolean en);
    boolean isEnabled();

  private:
    int _pin;
    long _interval;
    boolean _enabled;
    unsigned long _previousMillis;
    int _ledState; 
};
#endif
