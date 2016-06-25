#include "Arduino.h"
#include "piezo.h"

/* Knock Sensor library 
   author: Robert Gröber
   Version : 0.1
*/


Piezo::Piezo(int pin)
{
  //pinMode(pin, INPUT);
  _pin = pin;
  _threshold = 0.43; //default threshold
  _release = 50; // default release time
  _lastHit = 0;
  _sensorRaw = 0;
  _sensVolt = 0;
}

void Piezo::setThreshold(float volt)
{
  if(volt < 0 || volt > 5) return;
  _threshold = volt;
}

float Piezo::getThreshold()
{
  return _threshold;
}

void Piezo::setThresholdRaw(int raw)
{
  float volt = raw * (5.0 / 1023.0);
  setThreshold(volt);
}

void Piezo::setRelease(int releaseTime)
{
  if(releaseTime < 0 || releaseTime > 1000) return;
  _release = releaseTime;
}

/* read binary */
boolean Piezo::isHit()
{
  unsigned long now = millis();
  if ( (now - _lastHit) > _release )
  {
    // read the sensor and store it:
    _sensorRaw = analogRead(_pin);
    // calculate voltage
    _sensVolt = _sensorRaw * (5.0 / 1023.0);
  
    // if the sensor reading is greater than the threshold:
    if (_sensVolt > _threshold) 
    {
      _lastHit = now;
      return true;
    }
  }
  return false;
}

int Piezo::getRawValue()
{
  return _sensorRaw;
}

float Piezo::getValue()
{
  return _sensVolt;
}
