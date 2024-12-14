
// Code for Servo motor, PIR sensor and LED
// Servo motor moves when motion is deteced by PIR sensor

#include <Servo.h>  

Servo Servo1;   

int servoPin = 10;             // Servo motor pin
int ledPin = 8;                // LED pin
int pirPin = 7;                // PIR sensor pin


int lastPirVal = LOW;          // stores the value of the PIR, helps in detecting change
int pirVal;                    // the current value of the PIR sensor


unsigned long myTime;          // number of milliseconds passed since the Arduino started running the code itself
#define MINUTE 60000           // for converting milliseconds to a minute
#define SECOND 1000            // for converting milliseconds to a second
char printBuffer[128];         // messages that we want to display on serial monitor


void setup() {
  Servo1.attach(servoPin);     // initializing servo motor, telling it which pin is connected to the control signal
  pinMode(ledPin, OUTPUT);     // declare LED as output
  pinMode(pirPin, INPUT);      // declare PIR sensor as input
  Serial.begin(9600);          // initializing serial communication
}


void loop() {
  pirVal = digitalRead(pirPin); // read current input value from PIR sensor (HIGH or LOW)


  if (pirVal == HIGH) {        // if motion is detected
    digitalWrite(ledPin, HIGH); // turn LED on


    if (lastPirVal == LOW) {   // if there was no previous motion detected
      myTime = millis();       // Stores the current time in milliseconds
      sprintf(printBuffer, "%lu min %lu sec: Motion detected!", myTime / MINUTE, (myTime % MINUTE) / SECOND);
      Serial.println(printBuffer);
      lastPirVal = HIGH;
    }


    Servo1.write(180);           // Set servo to 90 degrees when motion is detected


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
