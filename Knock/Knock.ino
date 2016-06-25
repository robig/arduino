
/* Knock Sensor

   This sketch reads a piezo element to detect a knocking sound.
   It reads an analog pin and compares the result to a set threshold.
   If the result is greater than the threshold, it writes
   "knock" to the serial port, and toggles the LED on pin 13.

   The circuit:
	* + connection of the piezo attached to analog in 0
	* - connection of the piezo attached to ground
	* 1-megohm resistor attached from analog in 0 to ground

   http://www.arduino.cc/en/Tutorial/Knock

   created 25 Mar 2007
   by David Cuartielles <http://www.0j0.org>
   modified 30 Aug 2011
   by Tom Igoe

   This example code is in the public domain.

 */


// these constants won't change:
const int ledPin = 13;      // led connected to digital pin 13
const int knockSensor = A2; // the piezo is connected to analog pin 0
const int poti = A0;

// these variables will change:
int sensorRaw = 0;      // variable to store the value read from the sensor pin
float sensVolt = 0;
float lastSens = 0;

int ledState = LOW;         // variable used to store the last LED status, to toggle the light

int potiRaw = 0;
float potiVolt = 0;
float lastPoti = 0;

float changeThresh = 0.03;

void setup() {
  pinMode(ledPin, OUTPUT); // declare the ledPin as as OUTPUT
  Serial.begin(9600);       // use the serial port
}

void loop() {
  // read the sensor and store it:
  sensorRaw = analogRead(knockSensor);
  float sensVolt = sensorRaw * (5.0 / 1023.0);
  if(sensVolt > 0 && abs(sensVolt - lastSens) > changeThresh )
  {
      //Serial.print("Sensor: "); Serial.println(sensVolt);
      lastSens = sensVolt;
  }

  // read the poti
  potiRaw = analogRead(poti);
  
  float potiVolt = 5.0 - potiRaw * (5.0 / 1023.0);
  if(potiVolt > 0 && abs(potiVolt - lastPoti) > changeThresh )
  {
    Serial.print("Poti: "); Serial.println(potiVolt);
    lastPoti = potiVolt;
  }
  
  // if the sensor reading is greater than the threshold:
  if (sensVolt > potiVolt) {
    // toggle the status of the ledPin:
    ledState = !ledState;
    // update the LED pin itself:
    digitalWrite(ledPin, ledState);
    
    // send the string "Knock!" 
    Serial.print("knock ! ");
    Serial.print(" Sensor: "); Serial.print(sensVolt);
    Serial.print(" Poti: "); Serial.println(potiVolt);
    delay(50);
  }

  //lastSens = sensVolt;
  //lastPoti = potiVolt;
  
  //delay(100);  // delay to avoid overloading the serial port buffer
}

