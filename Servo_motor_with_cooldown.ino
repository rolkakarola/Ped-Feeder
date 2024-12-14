
// The same code as servo_motor but has cooldown 2s

#include <Servo.h>  


Servo Servo1;                 // Instance of Servo class


int servoPin = 10;             // Servo motor pin
int ledPin = 8;                // LED pin
int pirPin = 7;                // PIR sensor pin


int lastPirVal = LOW;          // stores the value of the PIR, helps in detecting change
int pirVal;                    // the current value of the PIR sensor


unsigned long myTime;          // number of milliseconds passed since the Arduino started running the code itself
#define MINUTE 60000           // for converting milliseconds to a minute
#define SECOND 1000            // for converting milliseconds to a second
#define SERVO_DELAY 2000       // servo delay of 2 seconds
#define SENSE_COOLDOWN 5000    // cooldown between senses (5 seconds)


unsigned long lastSenseTime = -SENSE_COOLDOWN; // set to allow immediate first activation
char printBuffer[128];         // messages that we want to display on serial monitor


void setup() {
  Servo1.attach(servoPin);     // initializing servo motor, telling it which pin is connected to the control signal
  pinMode(ledPin, OUTPUT);     // declare LED as output
  pinMode(pirPin, INPUT);      // declare PIR sensor as input
  Serial.begin(9600);          // initializing serial communication
}


void loop() {
  pirVal = digitalRead(pirPin); // read current input value from PIR sensor (HIGH or LOW)
  unsigned long currentTime = millis();


  // Check if motion is detected and cooldown period has passed
  if (pirVal == HIGH && (currentTime - lastSenseTime >= SENSE_COOLDOWN)) {
    digitalWrite(ledPin, HIGH); // turn LED on


    if (lastPirVal == LOW) {   // if there was no previous motion detected
      myTime = millis();       // Stores the current time in milliseconds
      sprintf(printBuffer, "%lu min %lu sec: Motion detected!", myTime / MINUTE, (myTime % MINUTE) / SECOND);
      Serial.println(printBuffer);
      lastPirVal = HIGH;
    }


    Servo1.write(90);           // Set servo to 90 degrees when motion is detected
    delay(SERVO_DELAY);         // Wait for 2 seconds
    Servo1.write(0);            // Return servo to 0 degrees after 2 seconds delay
    lastSenseTime = millis();   // Update the last sense time


  } else if (pirVal == LOW) {   // if no motion is detected
    digitalWrite(ledPin, LOW);  // turn LED off


    if (lastPirVal == HIGH) {   // if there was motion before
      myTime = millis();
      sprintf(printBuffer, "%lu min %lu sec: Motion ended!", myTime / MINUTE, (myTime % MINUTE) / SECOND);
      Serial.println(printBuffer);
      lastPirVal = LOW;
    }
    Servo1.write(0);           // Ensure servo is at 0 degrees when no motion is detected
  }
}
