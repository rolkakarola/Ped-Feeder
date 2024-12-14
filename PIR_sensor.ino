
// Code used only for PIR sensor and LED, it activates LED when motion is detected by PIR sensor

#include <Arduino.h>
#define MINUTE 60000 // for converting milliseconds to a minute
#define SECOND 1000 // for converting milliseconds to a second

int ledPin = 8;                // pin for the LED
int pirPin = 7;                // signal pin of the PIR sensor

int lastPirVal = LOW;          // the last value of the PIR sensor
int pirVal;                    // the current value of the PIR sensor

unsigned long myTime;          // number of milliseconds passed since the Arduino started running the code itself
char printBuffer[128];  

void setup() {
  pinMode(ledPin, OUTPUT);    // declare LED as output
  pinMode(pirPin, INPUT);     // declare PIR sensor as input

  Serial.begin(9600);
}

void loop() {
  pirVal = digitalRead(pirPin);  // read current input value

  if (pirVal == HIGH) { // movement detected  
    digitalWrite(ledPin, HIGH);  // turn LED on

    if (lastPirVal == LOW) { // if there was NO movement before
      myTime = millis();
      sprintf(printBuffer, "%lu min %lu sec: Motion detected!", myTime / MINUTE, myTime % MINUTE / SECOND); 
      Serial.println(printBuffer);
      lastPirVal = HIGH;
    }
  } else { // no movement detected
    digitalWrite(ledPin, LOW); // turn LED off

    if (lastPirVal == HIGH) { // if there was a movement before
      myTime = millis();
      sprintf(printBuffer, "%lu min %lu sec: Motion ended!", myTime / MINUTE, myTime % MINUTE / SECOND); 
      Serial.println(printBuffer);
      lastPirVal = LOW;
    }
  }
}
