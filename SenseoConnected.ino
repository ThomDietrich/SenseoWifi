
#include "SenseoLed.h"
#include "config.h"

SenseoLed senseoLed(ocSenseLedPin);
ledStateEnum ledState = LED_unknown; 
ledStateEnum prevLedState = LED_unknown; 

enum senseoStateEnum {
  SENSEO_unknown,
  SENSEO_OFF,
  SENSEO_HEATING,
  SENSEO_READY,
  SENSEO_BREWING,
  SENSEO_NOWATER
};
senseoStateEnum senseoState = SENSEO_unknown;
senseoStateEnum prevSenseoState = SENSEO_unknown;

unsigned long senseoStateLastChangeMillis;
const int senseoHeatingTime = 40; //in seconds
const int senseoTimingTolerance = 10; //in seconds


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
}


void loop() {
  // activate this to test your circuit and Senseo connections
  //testIO();
  
  senseoLed.updateState();
  if (senseoLed.hasChanged()) {
    Serial.print("LED state machine, new LED state: ");
    Serial.println(senseoLed.getStateAsString());
  }

  prevSenseoState = senseoState;
  updateSenseoState();
  if (senseoState != prevSenseoState) {
    Serial.print("Senseo state machine, new Senseo state: ");
    Serial.print(senseoStateToString(senseoState));
    Serial.print("   (time in old state: ");
    Serial.print((millis() - senseoStateLastChangeMillis + 500) / 1000);
    Serial.print("s)\n\n");
    senseoStateLastChangeMillis = millis();
  }
}


void ledChangedRoutine() {
  senseoLed.setLedChangedAt(millis());
  
  // Debugging and Setup: Uncomment the following to get LED pulse durations
  Serial.print("LED pulse duration: ");
  Serial.println(senseoLed.getLastPulseDuration());
}


void updateSenseoState() {
  switch (senseoState) {
    case SENSEO_unknown:
      if (ledState == LED_OFF) senseoState = SENSEO_OFF;
      else if (ledState == LED_SLOW) senseoState = SENSEO_HEATING;
      else if (ledState == LED_FAST) senseoState = SENSEO_NOWATER;
      else if (ledState == LED_ON) senseoState = SENSEO_READY;
      break;
    case SENSEO_OFF:
      if (ledState == LED_OFF) senseoState = SENSEO_OFF;
      else if (ledState == LED_SLOW) senseoState = SENSEO_HEATING;
      else if (ledState == LED_FAST) senseoState = SENSEO_NOWATER;
      else if (ledState == LED_ON) senseoState = SENSEO_READY;
      break;
    case SENSEO_HEATING:
      if (ledState == LED_OFF) senseoState = SENSEO_OFF;
      else if (ledState == LED_FAST) senseoState = SENSEO_NOWATER;
      else if (ledState == LED_ON) senseoState = SENSEO_READY;
      else if ((millis() - senseoStateLastChangeMillis) > 1000 * (senseoHeatingTime + senseoTimingTolerance)) {
        // Heating takes more time then expected, assume immediate Brew.
        senseoState = SENSEO_BREWING;
      }
      break;
    case SENSEO_READY:
      if (ledState == LED_ON) senseoState = SENSEO_READY;
      else if (ledState == LED_OFF) senseoState = SENSEO_OFF;
      else if (ledState == LED_SLOW) senseoState = SENSEO_BREWING;
      else if (ledState == LED_FAST) senseoState = SENSEO_NOWATER;
      break;
    case SENSEO_BREWING:
      if (ledState == LED_OFF) senseoState = SENSEO_OFF;
      else if (ledState == LED_FAST) {
        senseoState = SENSEO_NOWATER;
        Serial.println("Cup brewed!");
        //cup was brewed
      } else if (ledState == LED_ON) {
        senseoState = SENSEO_READY;
        Serial.println("Cup brewed!");
        //cup was brewed
      }
      break;
    case SENSEO_NOWATER:
      if (ledState == LED_FAST) senseoState = SENSEO_NOWATER;
      else if (ledState == LED_SLOW) senseoState = SENSEO_HEATING;
      else if (ledState == LED_ON) senseoState = SENSEO_READY;
      else if (ledState == LED_OFF) senseoState = SENSEO_OFF;
      break;  
  }
}


String senseoStateToString(senseoStateEnum state) {
  if (state == SENSEO_OFF) return "SENSEO_OFF";
  else if (state == SENSEO_HEATING) return "SENSEO_HEATING";
  else if (state == SENSEO_READY) return "SENSEO_READY";
  else if (state == SENSEO_BREWING) return "SENSEO_BREWING";
  else if (state == SENSEO_NOWATER) return "SENSEO_NOWATER";
  else return "SENSEO_unknown";
}


void testIO() {
  while (true) {
    Serial.print("\nHello Senseo :)\nCup: ");
    Serial.print(!digitalRead(cupDetectorPin));
    Serial.print("   LED: ");
    Serial.print(digitalRead(ocSenseLedPin));
    Serial.println();
    delay(2000);
  
    Serial.println("Testing Beeper.");
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
