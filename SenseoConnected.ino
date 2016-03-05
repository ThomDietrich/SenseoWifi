/*
   Hello World
   Simply prints Hello World to the serial monitor.
*/

int ocPressLeftPin = D1;
int ocPressRightPin = D2;
int ocPressPowerPin = D5;
int cupDetectorPin = D6;
int ocSenseLedPin = D7;
int beeperPin = D8;

bool ledChanged = 0;
unsigned long ledChangeMillis, prevLedChangeMillis;

int pulseDurLedSlow = 1000;
int pulseDurLedFast = 100;
int pulseDurTolerance = 10;
int pulseContThreshold = 2 * pulseDurLedSlow;

enum ledStateEnum {
  LED_unknown,
  LED_OFF,
  LED_SLOW,
  LED_FAST,
  LED_ON
};
ledStateEnum ledState = LED_unknown;
ledStateEnum prevLedState = LED_unknown;


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
  //testIO();
  prevLedState = ledState;
  updateLedState();
  if (ledState != prevLedState) Serial.println(ledState);
}


void ledChangedRoutine() {
  prevLedChangeMillis = ledChangeMillis;
  ledChangeMillis = millis();
  ledChanged = true;
  //Serial.print("LED pulse durration: ");
  //Serial.println(ledChangeMillis - prevLedChangeMillis);
}

void updateLedState() {
  if (ledChanged) {
    ledChanged = false;
    int durration = ledChangeMillis - prevLedChangeMillis;
    if ((pulseDurLedFast - pulseDurTolerance < durration) && (durration < pulseDurLedFast + pulseDurTolerance)) {
      if (ledState != LED_FAST) {
        ledState = LED_FAST;
        Serial.println("LED state changed to LED_FAST");
      }
    } else if ((pulseDurLedSlow - pulseDurTolerance < durration) && (durration < pulseDurLedSlow + pulseDurTolerance)) {
      if (ledState != LED_SLOW) {
        ledState = LED_SLOW;
        Serial.println("LED state changed to LED_SLOW");
      }
    }
  } else {
    int t = millis() - ledChangeMillis;
    if (( t > pulseContThreshold) && (t < 2 * pulseContThreshold)) {
      if (!digitalRead(ocSenseLedPin)) {
        if ((ledState != LED_ON) {
        ledState = LED_ON;
        Serial.println("LED state changed to LED_ON");
        }
      } else {
        if ((ledState != LED_ON) {
        ledState = LED_OFF;
        Serial.println("LED state changed to LED_OFF");
        }
      }
    }
  }
}


void testIO() {
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
