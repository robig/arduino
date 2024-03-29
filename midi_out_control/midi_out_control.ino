// midi_out_control.ino
// version 2015-04-09
// Arduino MIDI tutorial
// by Staffan Melin
// http://libremusicproduction.com/



// libraries

#include <SoftwareSerial.h>

// We will use the SoftwareSerial library instead of the Serial library, as this will let us control which pins our MIDI interface is connected to.

SoftwareSerial mySerial(2, 3); // RX, TX



// constants

const int pinDown = 4;
const int pinUp = 5;

const byte midiNoteOn = 144;
const byte midiNoteOff = 128;

const int midiSendDelay = 100; // give MIDI-device a short time to "digest" MIDI messages

// note lengths
const int bpm = 128; // tempo, BPM (beats per minute), a value that makes t16 an integer
const int t1 = 1024 * (bpm/64); // 1 whole beat = 512
const int t2 = t1/2; // 256
const int t4 = t2/2; // 128
const int t8 = t4/2; // 64
const int t16 = t8/2; // 32

const int voiceMidiChannel = 0;
const int voiceMidiPatch = 86;
const int voiceMidiVolume = 80;
const int voiceMidiPan = 100;
const int midiVelocity = 100;



//  Play a MIDI note

void midiNote(int aMidiCommand, int aMidiPitch, int aMidiVelocity) {
  mySerial.write(aMidiCommand);
  mySerial.write(aMidiPitch);
  mySerial.write(aMidiVelocity);
}

// Send MIDI command with 1 data byte

void midiData1(int aMidiCommand, int aData1) {
  mySerial.write(aMidiCommand);
  mySerial.write(aData1);
}

// Send MIDI command with 2 data bytes

void midiData2(int aMidiCommand, int aData1, int aData2) {
  mySerial.write(aMidiCommand);
  mySerial.write(aData1);
  mySerial.write(aData2);
}



void setup() {

  pinMode(pinDown, INPUT);
  pinMode(pinUp, INPUT);
  
  // setup SoftSerial for MIDI control
  mySerial.begin(31250);
  delay(midiSendDelay);

  // all sounds off on all 16 channels (can be synth-specific)
  for (int i = 0; i < 16; i++) {
    midiData2((0xB0 | i), 0x78, 0);
    delay(midiSendDelay);
    midiData1((0xB0 | i), 0x7B);
    delay(midiSendDelay);
  }

  // volume
  midiData2((0xB0 | voiceMidiChannel), 07, voiceMidiVolume);
  delay(midiSendDelay);

  // sound/patch
  midiData1((0xC0 | voiceMidiChannel), voiceMidiPatch);
  delay(midiSendDelay);

  // pan
  midiData2((0xB0 | voiceMidiChannel), 10, voiceMidiPan);
  delay(midiSendDelay);

}



void loop() {

  // if user presses 1st button play a short note  
  if (digitalRead(pinDown) == HIGH)
  {
    midiNote(midiNoteOn + voiceMidiChannel, 48, midiVelocity);
    delay(t8);
    midiNote(midiNoteOff + voiceMidiChannel, 48, midiVelocity); 
  }

  // if user presses 2nd button play a short note  
  if (digitalRead(pinUp) == HIGH)
  {
    midiNote(midiNoteOn + voiceMidiChannel, 51, midiVelocity);
    delay(t8);
    midiNote(midiNoteOff + voiceMidiChannel, 51, midiVelocity); 
  }

}
