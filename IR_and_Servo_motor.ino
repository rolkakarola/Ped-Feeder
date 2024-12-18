
// Combined IR sensor with servo motor
// There are two IR sensors one for detecting if bowl is full and the other for detecting if there is a food
// Servo motor will not move if bowl is full and has a cooldown

#include <Servo.h>  

// Pin for IR break beam sensors
const int sensorPin1 = 2; // IR sensor for portion detected
const int sensorPin2 = 3; // IR sensor for maximum capacity

// Pin for Servo motor, PIR sensor, and LED
Servo Servo1;
const int servoPin = 10;              // Servo motor pin
const int ledPin = LED_BUILTIN;       // LED on Arduino (Built-in)
const int pirPin = 7;                 // PIR sensor pin

// Variables for PIR sensor
int lastPirVal = LOW;      // stores the value of the PIR, helps in detecting change
int pirVal;                // the current value of the PIR sensor
unsigned long myTime;      // number of milliseconds passed since the Arduino started running the code itself

// Constants
#define MINUTE 60000       // for converting milliseconds to a minute
#define SECOND 1000        // for converting milliseconds to a second
#define SERVO_DELAY 2000   // Servo delay of 2 seconds
#define SENSE_COOLDOWN 5000 // Cooldown time of 5 seconds

unsigned long lastSenseTime = -SENSE_COOLDOWN; // Set to allow immediate first activation
char printBuffer[128];     // Messages that we want to display on serial monitor

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
  pirVal = digitalRead(pirPin); // read current input value from PIR sensor (HIGH or LOW)
  unsigned long currentTime = millis();

  // Check if motion is detected and cooldown period has passed
  if (pirVal == HIGH && (currentTime - lastSenseTime >= SENSE_COOLDOWN)) {
    digitalWrite(ledPin, HIGH); // turn LED on

    if (lastPirVal == LOW) {    // if there was no previous motion detected
      myTime = millis();        // Stores the current time in milliseconds
      sprintf(printBuffer, "%lu min %lu sec: =================== Motion detected! =================== ", myTime / MINUTE, (myTime % MINUTE) / SECOND);
      Serial.println(printBuffer);
      lastPirVal = HIGH;
    }

    // Check sensorState2 before moving the servo (if bowl is full, do not move the servo)
    int sensorState2 = digitalRead(sensorPin2);
    if (sensorState2 == HIGH) {  // Only move the servo if sensorState2 is HIGH (bowl not full)
      Servo1.write(180);         // Set servo to 180 degrees
      delay(SERVO_DELAY);        // Wait for 2 seconds
      Servo1.write(0);           // Return servo to 0 degrees after 2 seconds delay
      lastSenseTime = millis();  // Update the last sense time
    } else {
      Serial.println("Sensor2: Maximum capacity! Servo movement prevented.");
    }

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
