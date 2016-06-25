/*
  Piezo.h - Library piezo sensor
*/
#ifndef Piezo_h
#define Piezo_h

#include "Arduino.h"

class Piezo
{
  public:
    Piezo(int pin);
    boolean isHit();
    float getValue();
    int getRawValue();
    void setThreshold(float volt);
    void setRelease(int releaseTime);
  private:
    int _pin;
    float _threshold;
    int _release;
    long _lastHit;
    int _sensorRaw;
    float _sensVolt;
    
};

#endif
