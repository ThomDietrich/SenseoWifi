/*
  SenseoSM.cpp - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/

#include "SenseoSM.h"

SenseoSM::SenseoSM()
{
  // nothing to do here 
}

void SenseoSM::updateState(ledStateEnum ledState) {
  senseoStatePrev = senseoState;
  hasChanged = false;
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
      else if ((millis() - lastStateChangeMillis) > (1000 * (HeatingTime + HeatingTimeTol))) {
        // Heating takes more time then expected, assume immediate brew.
        Serial.println("Assuming immediate brew ...");
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
  if (senseoStatePrev != senseoState) {
    hasChanged = true;
    unsigned long now = millis();
    timeInLastState = (unsigned long)(now - lastStateChangeMillis);
    lastStateChangeMillis = now;
  }
}


bool SenseoSM::stateHasChanged() {
  // did the Senseo state change during last updateState() execution?
  return hasChanged;
}


int SenseoSM::getTimeInLastState() {
  // how long was the machine in the last state? (in seconds)
  return (timeInLastState + 500) / 1000;
}


senseoStateEnum SenseoSM::getState() {
  return senseoState;
}

String SenseoSM::getStateAsString() {
  if (senseoState == SENSEO_OFF) return "SENSEO_OFF";
  else if (senseoState == SENSEO_HEATING) return "SENSEO_HEATING";
  else if (senseoState == SENSEO_READY) return "SENSEO_READY";
  else if (senseoState == SENSEO_BREWING) return "SENSEO_BREWING";
  else if (senseoState == SENSEO_NOWATER) return "SENSEO_NOWATER";
  else return "SENSEO_unknown";
}

