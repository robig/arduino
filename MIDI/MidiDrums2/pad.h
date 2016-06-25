/*
  Morse.h - Library piezo sensor
*/
#ifndef Pad_h
#define Pad_h

#include "Arduino.h"
#include "piezo.h"


class Pad
{
  public:
    Pad();
    Pad(int pin, int note, int chan /*, midi::MidiInterface &midi*/ );
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
  private:
    int _pin;
    int _note;
    int _channel;
    int _release;
    int _noteVelocity;
    int _mapmax;
    unsigned long _startMillis;
    
    //midi::MidiInterface _midi;
    Piezo *_piezo;

    boolean _logging;

    
};

#endif
