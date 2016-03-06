/*
  config.h - Configuration for the SenseoWifi project.
  Created by Thomas Dietrich, 2016-03-05.
  Released under some license.
*/

#ifndef SenseoConfig_h
#define SenseoConfig_h

// circuit pin definitions
const int ocPressLeftPin = D1;
const int ocPressRightPin = D2;
const int ocPressPowerPin = D5;
const int cupDetectorPin = D6;
const int ocSenseLedPin = D7;
const int beeperPin = D8;

//characteristic values for LED state machine
//You can measure your timings by activating debugging in ledChangedRoutine()
const int pulseDurLedSlow = 1000;                   // duration of one pulse when LED is blinking slow in milliseconds (state LED_SLOW)
const int pulseDurLedFast = 100;                    // duration of one pulse when LED is blinking fast in milliseconds (state LED_FAST) 
const int pulseDurTolerance = 10;                   // tolerance for pulse duration. With the tested senseo, tolerance was not more than +-1ms 
const int pulseContThreshold = 2 * pulseDurLedSlow; // time before switching to continuous LED state (state LED_ON and state LED_OFF)

//characteristic values for Senseo state machine
const int HeatingTime = 40;           //in seconds
const int HeatingTimeTol = 10;        //in seconds
const int BrewingTimeOneCup = 55;     //in seconds
const int BrewingTimeTwoCup = 85;     //in seconds

#endif
