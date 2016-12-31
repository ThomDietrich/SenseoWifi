/*
  testIO.cpp - Testing routines for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/

#include "Arduino.h"
#include "pins.h"

inline void testIO() {
  Serial.println("\n================================");
  Serial.println("   ☕☕☕ Hello Senseo! ☕☕☕");
  Serial.println("   Executing Test Routine");
  Serial.println("==================================");
  while (true) {
    Serial.println("");
    
    Serial.print("Testing Cup Detector ... ");
    Serial.print(!digitalRead(cupDetectorPin) ? "detected" : "not detected");
    Serial.print(" (analog: ");
    Serial.print(analogRead(cupDetectorAnalogPin));
    Serial.println(")");
    delay(2000);
    
    Serial.print("Testing Status LED Probe ... ");
    Serial.print(!digitalRead(ocSenseLedPin) ? "lid" : "off");
    Serial.println();
    delay(2000);
    
    Serial.println("Testing Beeper ... ");
    tone(beeperPin, 1024, 1000);
    delay(2000);
    
    if (1) {
      Serial.println("Testing On/Off Button Triggering ... ");
      digitalWrite(ocPressPowerPin, HIGH);
      delay(200);
      digitalWrite(ocPressPowerPin, LOW);
      delay(2000);
    }
    
    if (1) {
      Serial.println("Testing Left Button Triggering ... ");
      digitalWrite(ocPressLeftPin, HIGH);
      delay(200);
      digitalWrite(ocPressLeftPin, LOW);
      delay(2000);
    }
    
    if (0) {
      Serial.println("Testing Right Button Triggering ...");
      digitalWrite(ocPressRightPin, HIGH);
      delay(200);
      digitalWrite(ocPressRightPin, LOW);
      delay(2000);
    }
  }
}
