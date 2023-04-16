/*
  LedObserver.cpp - Library for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/

#include "LedObserver.h"
#include <Homie.h>
#include <Arduino.h>
#include "constants.h"

// variable used both inside and outside the ISR should be volatile as they could be modified from outside the current code path
volatile static unsigned long s_ledChangeMillis = 0;
volatile static int s_pulseDuration = 0;    

void IRAM_ATTR LedObserver::ledChangedIsr()
{
    unsigned long now = millis();
    if (now - s_ledChangeMillis <= LedIgnoreChangeDuration)
        return; // simple debouncer

    s_pulseDuration = now - s_ledChangeMillis;    
    s_ledChangeMillis = now;
}

LedObserver::LedObserver(HomieNode &senseoNode, int pin)
    : ledPin(pin), senseoNode(senseoNode)
{   
    // just here to avoid two instance of the class at the same time for now
    // could be moved to a static member if this need to be access from a static method
    static LedObserver * s_instance = nullptr; 

    pinMode(ledPin, INPUT_PULLUP);
    assert(s_instance == nullptr); // You can't have two instance of this class
    s_instance = this;
    s_ledChangeMillis = millis();
}

void LedObserver::onMqttReady()
{
    senseoNode.setProperty("ledState").send(getStateAsString());
}

void LedObserver::attachInterrupt()
{
    ::attachInterrupt(digitalPinToInterrupt(ledPin), LedObserver::ledChangedIsr, CHANGE);
}

void LedObserver::detachInterrupt()
{
    ::detachInterrupt(digitalPinToInterrupt(ledPin));
}

void LedObserver::updateState()
{
    unsigned long now = millis();
    // disable interrupts to avoid race condition as the interrupts could occured while we are executing updateState
    noInterrupts();
    unsigned long ledChangeMillis = s_ledChangeMillis;
    int pulseDuration = s_pulseDuration;    
    interrupts();

    ledStateEnum ledStatePrev = ledState;
    // Check if the led change since our last update
    if (ledChangeMillis >= lastUpdateMillis)
    {
        // When there was an interrupt from the Senseo LED pin
        if (lastPulseDuration != pulseDuration)
            Homie.getLogger() << "LED observer, last pulse duration: " << pulseDuration << endl;

        // decide if LED is blinking fast or slow
        if (abs(pulseDuration - pulseDurLedFast) < pulseDurTolerance)
        {
            ledState = LED_FAST;
        }
        else if (abs(pulseDuration - pulseDurLedSlow) < pulseDurTolerance)
        {
            ledState = LED_SLOW;
        }
        else
        {
            // Nothing to do here.
            // pulseDuration could be below (user interaction) or above (end of a continuous state) the known times.
            // No actions needed.
        }
        lastPulseDuration = pulseDuration;
    }
    // decide if LED is not blinking but in a continuous state
    // this should be safe in case of very long frame because ledChangeMillis is updated from the interrupt (which will kick on many time during this long frame)
    int t = now - ledChangeMillis;
    if ((t > pulseContThreshold) && (t < 3 * pulseContThreshold || ledState == LED_unknown)) //When the machine start, it could happen that the first "pulse" was longer than pulseContThreshold
    {
        ledState = !digitalRead(ledPin) ? LED_ON : LED_OFF;
    }

    if (ledStatePrev != ledState)
    {
        Homie.getLogger() << "LED state machine, new LED state: " << getStateAsString() << endl;
        senseoNode.setProperty("ledState").send(getStateAsString());
    }

    lastUpdateMillis = now;    
}

ledStateEnum LedObserver::getState() const
{
    return ledState;
}
