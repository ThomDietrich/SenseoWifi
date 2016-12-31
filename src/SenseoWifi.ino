/*
  SenseoWifi.ino - base file for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/

#include "SenseoLed.h"
#include "SenseoSM.h"
#include "SenseoControl.h"
#include "Cup.h"
#include "pins.h"
#include "constants.h"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

SenseoLed mySenseoLed(ocSenseLedPin);
SenseoSM mySenseoSM;
Cup myCup(cupDetectorPin, beeperPin);
SenseoControl myControl(ocPressPowerPin, ocPressLeftPin, ocPressRightPin);

const char *WifiSsid = "AWF1877zh";    // cannot be longer than 32 characters!
const char *WifiPass = "nurzen010!";   //
IPAddress server(85, 119, 83, 194);

WiFiClient wclient;
PubSubClient client(wclient, server);


void ledChangedRoutine() {
  // will get called by the LED changed interrupt
  mySenseoLed.pinStateToggled();

  // Debugging and Setup: Uncomment the following to get LED pulse durations
  //Serial.print("LED pulse duration: ");
  //Serial.println(mySenseoLed.getLastPulseDuration());
}

void cupDetectorRoutine() {
  //debounce!?
  myCup.updateState();
  //Serial.print("Cup Detector: ");
  //Serial.println(myCup.getState());
}

void callback(const MQTT::Publish& pub) {
  //Serial.print(pub.topic());
  //Serial.print(": ");
  if (pub.has_stream()) {
    Serial.println("(too long)");
  } else
    myControl.setMqttMessage(pub.payload_string());
  //Serial.println(pub.payload_string());
}


void WiFiMQTTConnect() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(WifiSsid);
    Serial.println("...");
    WiFi.begin(WifiSsid, WifiPass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }

  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      if (client.connect("arduinoClient")) {
        //Serial.println("Connected to MQTT server");
        client.set_callback(callback);
        client.publish("SenseoWifi/w66a/msg", "Senseo Connected! :)");
        client.subscribe("SenseoWifi/w66a/command");
      }
    }
    if (client.connected()) client.loop();
  }
}


void testIO() {
  Serial.println("\n=======================");
  Serial.println("   Hello Senseo :)");
  Serial.println("   (testing all inputs and outputs)");
  Serial.println("=======================");
  while (true) {
    Serial.print("\nCup: ");
    Serial.print(!digitalRead(cupDetectorPin));
    Serial.print(" (");
    Serial.print(analogRead(cupDetectorAnalogPin));
    Serial.print(")   Senseo status LED: ");
    Serial.print(!digitalRead(ocSenseLedPin));
    Serial.println();
    delay(2000);

    Serial.println("Testing Beeper ...");
    tone(beeperPin, 1024, 1000);
    delay(2000);

    if (0) {
      Serial.println("Testing On/Off Button ...");
      digitalWrite(ocPressPowerPin, HIGH);
      delay(200);
      digitalWrite(ocPressPowerPin, LOW);
      delay(2000);
    }

    if (0) {
      Serial.println("Testing Left Button ...");
      digitalWrite(ocPressLeftPin, HIGH);
      delay(200);
      digitalWrite(ocPressLeftPin, LOW);
      delay(2000);
    }

    if (0) {
      Serial.println("Testing Right Button ...");
      digitalWrite(ocPressRightPin, HIGH);
      delay(200);
      digitalWrite(ocPressRightPin, LOW);
      delay(2000);
    }
  }
}


void setup() {
  Serial.begin(9600);

  pinMode(ocPressLeftPin, OUTPUT);
  pinMode(ocPressRightPin, OUTPUT);
  pinMode(ocPressPowerPin, OUTPUT);
  pinMode(ocSenseLedPin, INPUT_PULLUP);

  pinMode(cupDetectorPin, INPUT_PULLUP);
  pinMode(cupDetectorAnalogPin, INPUT);
  pinMode(beeperPin, OUTPUT);

  digitalWrite(ocPressPowerPin, LOW);
  digitalWrite(ocPressLeftPin, LOW);
  digitalWrite(ocPressRightPin, LOW);

  attachInterrupt(digitalPinToInterrupt(ocSenseLedPin), ledChangedRoutine, CHANGE);
  attachInterrupt(digitalPinToInterrupt(cupDetectorPin), cupDetectorRoutine, CHANGE);

  Serial.println("\n");

  // uncomment to test your circuit and Senseo connections
  //testIO();
}


void loop() {
  WiFiMQTTConnect();

  mySenseoLed.updateState();
  if (mySenseoLed.hasChanged()) {
    Serial.print("LED state machine, new LED state: ");
    Serial.println(mySenseoLed.getStateAsString());
  }

  mySenseoSM.updateState(mySenseoLed.getState());
  if (mySenseoSM.stateHasChanged()) {
    Serial.print("(time in last state: ");
    Serial.print(mySenseoSM.getTimeInLastState());
    Serial.print("s)\n");
    Serial.print("Senseo state machine, new Senseo state: ");
    Serial.print(mySenseoSM.getStateAsString());
    Serial.print("\n\n");
  }

  if (mySenseoSM.stateHasChanged()) {
    String s = "in last state: ";
    client.publish("SenseoWifi/w66a/time", s + mySenseoSM.getTimeInLastState());
    client.publish("SenseoWifi/w66a/state", mySenseoSM.getStateAsString());
  }

  //TODO:
  // Publish Senseo state every x minutes
    
  if (mySenseoSM.stateHasChanged()) {
    if (mySenseoSM.getState() == SENSEO_READY) {
      tone(beeperPin, 1024, 500);
    }
    if (mySenseoSM.getState() == SENSEO_NOWATER) {
      tone(beeperPin, 2048, 1000);
    }
  }

  if (myControl.hasMqttMsg()) {
    if (myControl.reactOnMqttMsg(mySenseoSM.getState())) {
      Serial.println("MQTT: command accepted");
      client.publish("SenseoWifi/w66a/response", "command accepted");
    } else {
      Serial.println("MQTT: unknown command");
      client.publish("SenseoWifi/w66a/response", "unknown command");
    }
  }
}


