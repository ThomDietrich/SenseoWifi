/*
  testIO.cpp - Testing routines for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/

#include "Homie.h"
#include "pins.h"

inline void testIO() {
  Serial.println("\n================================");
  Serial.println("   ☕☕☕ Hello Senseo! ☕☕☕");
  Serial.println("   Executing Test Routine");
  Serial.println("==================================");
  while (true) {
    Serial.println("");
    
    Serial.print("Testing Reset Button ... ");
    Serial.print(digitalRead(resetButtonPin) ? "HIGH" : "LOW");
    Serial.println();
    delay(1000);
    
    Serial.print("Testing Cup Detector ... ");
    Serial.print(!digitalRead(cupDetectorPin) ? "detected" : "not detected");
    Serial.print(" (analog: ");
    Serial.print(analogRead(cupDetectorAnalogPin));
    Serial.println(")");
    delay(1000);
    
    Serial.print("Testing Status LED Probe ... ");
    Serial.print(!digitalRead(ocSenseLedPin) ? "lid" : "off");
    Serial.println();
    delay(1000);
    
    Serial.println("Testing Beeper ... ");
    tone(beeperPin, 1024, 1000);
    delay(1000);
    
    if (1) {
      Serial.println("Testing On/Off Button Triggering ... ");
      digitalWrite(ocPressPowerPin, HIGH);
      delay(50);
      digitalWrite(ocPressPowerPin, LOW);
      delay(1000);
    }
    
    if (1) {
      Serial.println("Testing Left Button Triggering ... ");
      digitalWrite(ocPressLeftPin, HIGH);
      delay(50);
      digitalWrite(ocPressLeftPin, LOW);
      delay(1000);
    }
    
    if (0) {
      Serial.println("Testing Right Button Triggering ...");
      digitalWrite(ocPressRightPin, HIGH);
      delay(50);
      digitalWrite(ocPressRightPin, LOW);
      delay(1000);
    }
  }
}
