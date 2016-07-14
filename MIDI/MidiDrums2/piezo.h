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
    float getVoltage();
    int getRawValue();
    void setThreshold(unsigned short volt);
    unsigned short getThreshold();
    
    void setRelease(int releaseTime);
  private:
    int _pin;
    unsigned short _threshold;
    unsigned short _release;
    long _lastHit;
    int _sensorRaw;
    float _sensVolt;
    
};

#endif
