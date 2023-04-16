#include "HwTimerLedObserver.h"
#include "Arduino.h"
#include "constants.h"
#include <Homie.h>

// variable use both inside and outside the ISR should be volatile as they could be modified from outside the current code path
volatile static ledStateEnum s_ledState = LED_unknown;
// volatile static int pulseDuration = 0; // don't forget to remove the variable declaration in ledChangedIsr()

static const uint32_t s_timerTick = uint32_t(float((pulseDurLedSlow + 100) /*ms*/ * 1000 /*to us*/) / 3.2 /*to tick*/);
static int s_ledPin = -1;

ledStateEnum HwTimerLedObserver::getState() const
{
    return s_ledState;
}

void IRAM_ATTR HwTimerLedObserver::ledChangedIsr()
{
    static unsigned long ledChangeMillis = 0;

    unsigned long now = millis();
    int pulseDuration = now - ledChangeMillis;
    if (abs(pulseDuration - pulseDurLedFast) < pulseDurTolerance)
        s_ledState = LED_FAST;
    else if (abs(pulseDuration - pulseDurLedSlow) < pulseDurTolerance)
        s_ledState = LED_SLOW;
    else
        pulseDuration = 0; // longer pulse duration means "no pulse" (continuous on or continuous off)

    // each time the led blink, we push back the timer
    timer1_write(s_timerTick);
    ledChangeMillis = now;
}

void IRAM_ATTR HwTimerLedObserver::timerElapseIsr()
{
    s_ledState = !digitalRead(s_ledPin) ? LED_ON : LED_OFF;
}

HwTimerLedObserver::HwTimerLedObserver(HomieNode &senseoNode, int pin)
    : ledPin(pin), senseoNode(senseoNode)
{
    assert(s_ledPin == -1); // you can't create two HwTimerLedObserver instance or terrible things will most likely happen
    pinMode(ledPin, INPUT_PULLUP);
    s_ledPin = ledPin;
}

void HwTimerLedObserver::attachInterrupt()
{
    ::attachInterrupt(digitalPinToInterrupt(ledPin), HwTimerLedObserver::ledChangedIsr, CHANGE);
    timer1_attachInterrupt(HwTimerLedObserver::timerElapseIsr);
    timer1_enable(TIM_DIV256, TIM_EDGE, TIM_SINGLE);
    timer1_write(s_timerTick);
}

void HwTimerLedObserver::detachInterrupt()
{
    ::detachInterrupt(digitalPinToInterrupt(ledPin));
    timer1_detachInterrupt();
    timer1_disable();
}

void HwTimerLedObserver::updateState()
{
    /*static int lastPulseDuration = 0;
    if (lastPulseDuration != pulseDuration)
    {
        //pulseDuration is not thread safe. So the debug value display here could be glitchy every now and then
        if (pulseDuration > 0)
        {
            //pulse duration of 0 means continuous state
            Homie.getLogger() << "LED observer, last pulse duration: " << pulseDuration << endl;
        }
        lastPulseDuration = pulseDuration;
    }*/

    if (s_ledState != prevState)
    {
        Homie.getLogger() << "LED state machine, new LED state: " << getStateAsString() << endl;
        senseoNode.setProperty("ledState").send(getStateAsString());
        prevState = s_ledState;
    }
}
