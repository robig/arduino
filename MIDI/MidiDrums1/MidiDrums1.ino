/* MIDI Drums v1

 */

#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <midi_Settings.h>
#include <SoftwareSerial.h>

// configuration:
const int pinLed = 13;      // led pin
const int knockSensor = A2; // the piezo is connected to this analog pin
const int thresholdAdd = 1022;  // threshold value to decide when the detected sound is a knock or not
const int pinPoti = A0;
const int pinMidiIn = 2;
const int pinMidiOut = 3;

// these variables will change:
int sensorReading = 0;      // variable to store the value read from the sensor pin
int lastSens = 0;
int ledState = LOW;         // variable used to store the last LED status, to toggle the light
int threshold = 0;
int lastPoti = 0;

const int midiChan = 1;
const int noteValue = 32;
const long noteDuration = 1000; //ms
unsigned long startMillis = 0;
unsigned long currentMillis = 0;
int noteVelocity = 0;

SoftwareSerial midiSerial(pinMidiIn,pinMidiOut); //Midi Port
MIDI_CREATE_INSTANCE(SoftwareSerial, midiSerial, MIDI);

void setup() {
  pinMode(pinLed, OUTPUT); // declare the ledPin as as OUTPUT
  Serial.begin(9600);       // use the serial port
}

void loop() {
  // read the sensor and store it in the variable sensorReading:
  sensorReading = analogRead(knockSensor);
  

  if(sensorReading < lastSens-2 || sensorReading > lastSens+2)
  {
    float voltage = 5.0 - sensorReading * (5.0 / 1023.0);
    Serial.print("Sensor: "); Serial.println(voltage);
  }

  threshold = analogRead(pinPoti);
  
  //Output threashold values
  if(threshold < lastPoti-2 || threshold > lastPoti+2)
  {
    float voltage2 = 5.0 - threshold * (5.0 / 1023.0);
    Serial.print("Poti: "); Serial.println(voltage2);
  }
  
  // if the sensor reading is greater than the threshold:
  if (sensorReading >= threshold) {
    // toggle the status of the ledPin:
    ledState = !ledState;
    // update the LED pin itself:
    digitalWrite(pinLed, ledState);

    startMillis = millis();
    noteVelocity = (sensorReading) ;
    Serial.print("Note On "); Serial.println(noteVelocity);
    MIDI.sendNoteOn(noteValue,noteVelocity,midiChan);
  }

  lastSens = sensorReading;
  lastPoti = threshold;

  currentMillis = millis();
  if (noteVelocity > 0 && currentMillis - startMillis >= noteDuration) {
    
    MIDI.sendNoteOn(noteValue,0,midiChan);
    Serial.println("Note Off");
    noteVelocity = 0;
  }
  
  delay(100);  // delay to avoid overloading the serial port buffer
}

