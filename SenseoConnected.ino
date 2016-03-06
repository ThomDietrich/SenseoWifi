

#include "config.h"
#include "SenseoLed.h"
#include "SenseoSM.h"

SenseoLed mySenseoLed(ocSenseLedPin);
SenseoSM mySenseoSM;

void setup() {
  Serial.begin(9600);

  pinMode(ocPressLeftPin, OUTPUT);
  pinMode(ocPressRightPin, OUTPUT);
  pinMode(ocPressPowerPin, OUTPUT);
  pinMode(ocSenseLedPin, INPUT_PULLUP);

  pinMode(cupDetectorPin, INPUT_PULLUP);
  pinMode(beeperPin, OUTPUT);

  digitalWrite(ocPressPowerPin, LOW);
  digitalWrite(ocPressLeftPin, LOW);
  digitalWrite(ocPressRightPin, LOW);

  attachInterrupt(digitalPinToInterrupt(ocSenseLedPin), ledChangedRoutine, CHANGE);
  
  Serial.println("\n");

  // uncomment to test your circuit and Senseo connections
  //testIO();
}


void ledChangedRoutine() {
  // will get called by the LED changed interrupt
  mySenseoLed.pinStateToggled();
  
  // Debugging and Setup: Uncomment the following to get LED pulse durations
  //Serial.print("LED pulse duration: ");
  //Serial.println(mySenseoLed.getLastPulseDuration());
}


void loop() {  
  mySenseoLed.updateState();
  if (mySenseoLed.hasChanged()) {
    Serial.print("LED state machine, new LED state: ");
    Serial.println(mySenseoLed.getStateAsString());
  }
  
  mySenseoSM.updateState(mySenseoLed.getState());
  if (mySenseoSM.stateHasChanged()) {
    Serial.print("Senseo state machine, new Senseo state: ");
    Serial.print(mySenseoSM.getStateAsString());
    Serial.print(" (time in old state: ");
    Serial.print(mySenseoSM.getTimeInLastState());
    Serial.print("s)\n\n");
  }
}


void testIO() {
  while (true) {
    Serial.print("\nHello Senseo :)\nCup: ");
    Serial.print(!digitalRead(cupDetectorPin));
    Serial.print("   Senseo status LED: ");
    Serial.print(digitalRead(ocSenseLedPin));
    Serial.println();
    delay(2000);
  
    Serial.println("Testing Beeper ...");
    tone(beeperPin, 1024, 1000);
    delay(2000);
  
    if (1) {
      Serial.println("Testing On/Off Button ...");
      digitalWrite(ocPressPowerPin, HIGH);
      delay(200);
      digitalWrite(ocPressPowerPin, LOW);
      delay(2000);
    }
  
    if (1) {
      Serial.println("Testing Left Button ...");
      digitalWrite(ocPressLeftPin, HIGH);
      delay(200);
      digitalWrite(ocPressLeftPin, LOW);
      delay(2000);
    }
  
    if (1) {
      Serial.println("Testing Right Button ...");
      digitalWrite(ocPressRightPin, HIGH);
      delay(200);
      digitalWrite(ocPressRightPin, LOW);
      delay(2000);
    }
  }
}


