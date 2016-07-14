#include "Arduino.h"
#include "blinker.h"

Blinker::Blinker()
{
}

Blinker::Blinker(int pin)
{
  _enabled = false;
  _pin = pin;
  _interval = 500;
  _ledState = LOW;
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, _ledState);
}

void Blinker::process()
{
  if(!_enabled) return;
  unsigned long currentMillis = millis();

  if (currentMillis - _previousMillis >= _interval) {
    // save the last time you blinked the LED
    _previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (_ledState == LOW) {
      _ledState = HIGH;
    } else {
      _ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(_pin, _ledState);
  }
}

void Blinker::setInterval(long iv)
{
  _interval = iv;
}
long Blinker::getInterval()
{
  return _interval;
}

void Blinker::setEnabled(boolean en)
{
  _enabled = en;
  if(!en) 
  {
    _ledState = LOW;
    digitalWrite(_pin, _ledState);
  }
}
boolean Blinker::isEnabled()
{
  return _enabled;
}

