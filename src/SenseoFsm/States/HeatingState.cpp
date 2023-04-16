#include "HeatingState.h"

#include "LedObserver/ILedObserver.h"
#include "constants.h"
#include "NoWaterState.h"
#include "ReadyState.h"
#include "OffState.h"
#include "BrewingState.h"
#include "../Components/SenseoLedComponent.h"
#include "../Components/ControlComponent.h"

void HeatingState::onEnter(StateId previousState) 
{
  EXECUTE_IF_COMPONENT_EXIST(SenseoLedComponent,blink(1000));
}

void HeatingState::onUpdate() 
{
  ledStateEnum ledState = senseoLed.getState();

  if (ledState == LED_OFF)  changeState<OffState>();
  else if (hasOffCommands()) processOffCommands();
  else if (ledState == LED_FAST) changeState<NoWaterState>();
  else if (ledState == LED_ON) changeState<ReadyState>();
  else if (getTimeInState() > (1000 * (HeatingTime + HeatingTimeTol))) 
  {
    // Heating takes more time then expected, assume immediate brew.
    changeState<BrewingState>();
  }
}