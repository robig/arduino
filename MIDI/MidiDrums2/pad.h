/*
  pad.h - Library for drum pad using piezo sensor (and piezo library)
*/
#ifndef Pad_h
#define Pad_h

#include "Arduino.h"
#include "piezo.h"
#include "logger.h"

class Pad
{
  public:
    Pad();
    Pad(int pin, int note, int chan);
    boolean process();

    int getNote();
    int getChannel();
    int getVelocity();
    
    Piezo *getPiezo();
    
    void setRelease(int releaseTime);
    void setMapMaxValue(int raw);
    int getMapMaxValue();

    int mapRawValue(int raw);

    void enableLogging(boolean dbg);

    void resetBuffer();
  private:
    int _pin;
    int _note;
    int _channel;
    int _release;
    int _noteVelocity;
    int _mapmax;
    unsigned long _startMillis;

    unsigned short _buffer[];
    unsigned short _bufferIndex;
    unsigned short _bufferMax;
    
    Piezo *_piezo;

    boolean _logging;    
};

#endif
