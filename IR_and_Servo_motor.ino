
// Combined IR sensor with servo motor, code neeeds to be futher developed

#include <Servo.h>

// Pin definitions for IR break beam sensors
const int sensorPin1 = 2; // IR break beam sensor connected to pin 2
const int sensorPin2 = 3; // IR break beam sensor connected to pin 3

// Pin definitions for Servo motor, PIR sensor, and LED
Servo Servo1;
const int servoPin = 10;  // Servo motor pin
const int ledPin = 8;     // LED pin
const int pirPin = 7;     // PIR sensor pin

// Variables for PIR sensor
int lastPirVal = LOW;      // stores the value of the PIR, helps in detecting change
int pirVal;                // the current value of the PIR sensor
unsigned long myTime;      // number of milliseconds passed since the Arduino started running the code itself

// Constants
#define MINUTE 60000       // for converting milliseconds to a minute
#define SECOND 1000        // for converting milliseconds to a second
char printBuffer[128];     // messages that we want to display on serial monitor

void setup() {
  // Setup for IR break beam sensors
  pinMode(sensorPin1, INPUT_PULLUP); // Use internal pull-up resistor for the first sensor
  pinMode(sensorPin2, INPUT_PULLUP); // Use internal pull-up resistor for the second sensor

  // Setup for Servo motor, PIR sensor, and LED
  Servo1.attach(servoPin);     // initializing servo motor, telling it which pin is connected to the control signal
  pinMode(ledPin, OUTPUT);     // declare LED as output
  pinMode(pirPin, INPUT);      // declare PIR sensor as input

  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("System Initialized: IR Sensors, PIR Sensor, Servo Motor");
}

void loop() {
  // --- IR Break Beam Sensors ---
  int sensorState1 = digitalRead(sensorPin1);
  int sensorState2 = digitalRead(sensorPin2);

  if (sensorState1 == LOW) {
    Serial.println("Portion detected.");
  }

  if (sensorState2 == LOW) {
    Serial.println("Maximum capacity! No more allowed!!");
  }

  if (sensorState1 == HIGH && sensorState2 == HIGH) {
    Serial.println("No objects detected.");
  }

  delay(200); // Small delay for IR sensor readability

  // --- PIR Sensor and Servo Motor ---
  pirVal = digitalRead(pirPin); // read current input value from PIR sensor (HIGH or LOW)

  if (pirVal == HIGH) {        // if motion is detected
    digitalWrite(ledPin, HIGH); // turn LED on

    if (lastPirVal == LOW) {   // if there was no previous motion detected
      myTime = millis();       // Stores the current time in milliseconds
      sprintf(printBuffer, "%lu min %lu sec: Motion detected!", myTime / MINUTE, (myTime % MINUTE) / SECOND);
      Serial.println(printBuffer);
      lastPirVal = HIGH;
    }

    Servo1.write(180);           // Set servo to 180 degrees when motion is detected

  } else {                      // if no motion is detected
    digitalWrite(ledPin, LOW);  // turn LED off

    if (lastPirVal == HIGH) {   // if there was motion before
      myTime = millis();
      sprintf(printBuffer, "%lu min %lu sec: Motion ended!", myTime / MINUTE, (myTime % MINUTE) / SECOND);
      Serial.println(printBuffer);
      lastPirVal = LOW;
    }

    Servo1.write(0);           // Return servo to 0 degrees when no motion is detected
  }
}
