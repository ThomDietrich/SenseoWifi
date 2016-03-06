/*
  SenseoConnected.ino - base file for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/

#include "SenseoLed.h"
#include "SenseoSM.h"
#include "Cup.h"
#include "config.h"

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

SenseoLed mySenseoLed(ocSenseLedPin);
SenseoSM mySenseoSM;
Cup cup(cupDetectorPin, beeperPin);

const char *ssid =  "xxxxxxxxxx";    // cannot be longer than 32 characters!
const char *pass =  "xxxxxxxxxx";   //
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
  cup.updateState();
  //Serial.print("Cup Detector: ");
  //Serial.println(cup.getState());
}


String subMsg = "";

void callback(const MQTT::Publish& pub) {
  //Serial.print(pub.topic());
  //Serial.print(": ");
  if (pub.has_stream()) {
    Serial.println("(too long)");
  } else
    subMsg = pub.payload_string();
  //Serial.println(pub.payload_string());
}


void WiFiMQTTConnect() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to ");
    Serial.print(ssid);
    Serial.println("...");
    WiFi.begin(ssid, pass);

    if (WiFi.waitForConnectResult() != WL_CONNECTED)
      return;
    Serial.println("WiFi connected");
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    if (!client.connected()) {
      if (client.connect("arduinoClient")) {
        Serial.println("Connected to MQTT server");
        client.set_callback(callback);
        client.publish("SenseoConnected/w66a/msg", "Senseo Connected! :)");
        client.subscribe("SenseoConnected/w66a/command");
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
    Serial.print("Senseo state machine, new Senseo state: ");
    Serial.print(mySenseoSM.getStateAsString());
    Serial.print(" (time in old state: ");
    Serial.print(mySenseoSM.getTimeInLastState());
    Serial.print("s)\n\n");
  }

  if (mySenseoSM.stateHasChanged()) {
    String s = "in last state: ";
    client.publish("SenseoConnected/w66a/time", s + mySenseoSM.getTimeInLastState());
    client.publish("SenseoConnected/w66a/state", mySenseoSM.getStateAsString());
  }

  if (mySenseoSM.stateHasChanged()) {
    if (mySenseoSM.getState() == SENSEO_READY) {
      tone(beeperPin, 1024, 500);
    }
    if (mySenseoSM.getState() == SENSEO_NOWATER) {
      tone(beeperPin, 2048, 1000);
    }
  }

  if (subMsg != "") {
    Serial.print("Nachricht: ");
    Serial.println(subMsg);
    if (subMsg == "ON") {
      if (mySenseoSM.getState() == SENSEO_OFF) {
        digitalWrite(ocPressPowerPin, HIGH);
        delay(100);
        Serial.println("MQTT: command accepted");
        client.publish("SenseoConnected/w66a/response", "accepted");
        digitalWrite(ocPressPowerPin, LOW);
      }
    } else if (subMsg == "OFF") {
      if (mySenseoSM.getState() != SENSEO_OFF) {
        digitalWrite(ocPressPowerPin, HIGH);
        delay(100);
        Serial.println("MQTT: command accepted");
        client.publish("SenseoConnected/w66a/response", "accepted");
        digitalWrite(ocPressPowerPin, LOW);
      }
    } else if (subMsg == "1-cup") {
      if (mySenseoSM.getState() == SENSEO_READY) {
        digitalWrite(ocPressLeftPin, HIGH);
        delay(100);
        Serial.println("MQTT: command accepted");
        client.publish("SenseoConnected/w66a/response", "accepted");
        digitalWrite(ocPressLeftPin, LOW);
      }
    } else if (subMsg == "2-cup") {
      if (mySenseoSM.getState() == SENSEO_READY) {
        digitalWrite(ocPressRightPin, HIGH);
        delay(100);
        digitalWrite(ocPressRightPin, LOW);
        Serial.println("MQTT: command accepted");
        client.publish("SenseoConnected/w66a/response", "accepted");
      }
    } else {
      Serial.println("MQTT: unknown command");
      client.publish("SenseoConnected/w66a/response", (String)("unknown command" + subMsg));
    }
    subMsg = "";
  }
}


