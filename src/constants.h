/*
  config.h - Configuration for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/

#ifndef SenseoConstants_h
#define SenseoConstants_h

// LED state machine
// Measure your timings by activating debugging in ledChangedRoutine()
static const int pulseDurLedSlow = 1000;                   // duration of one pulse when LED is blinking slow in milliseconds (state LED_SLOW)
static const int pulseDurLedFast = 100;                    // duration of one pulse when LED is blinking fast in milliseconds (state LED_FAST)
static const int pulseDurTolerance = 10;                   // tolerance for pulse duration. With the tested senseo, tolerance was not more than +-1ms
static const int pulseContThreshold = 2 * pulseDurLedSlow; // time before switching to continuous LED state (state LED_ON and state LED_OFF)

// Senseo state machine (in seconds)
static const int HeatingTime = 60;
static const int HeatingTimeTol = 10;
static const int BrewingTime1Cup = 55;
static const int BrewingTime2Cup = 85;

// Debouncing (in milliseconds)
static const int CupDebounceInterval = 500;
static const int LedIgnoreChangeDuration = 5;

// Duration of button "press" (in milliseconds)
static const int pressDuration = 100;

#endif
