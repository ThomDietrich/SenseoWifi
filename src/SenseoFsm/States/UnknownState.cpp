#include <Homie.h>

#include "UnknownState.h"
#include "LedObserver/ILedObserver.h"
#include "HeatingState.h"
#include "NoWaterState.h"
#include "ReadyState.h"
#include "OffState.h"
#include "../Components/SenseoLedComponent.h"
#include "../Components/BuzzerComponent.h"

void UnknownState::onEnter(StateId previousState) 
{
    clearCommands(CommandComponent::All);
    EXECUTE_IF_COMPONENT_EXIST(SenseoLedComponent,blink(200,1000));

    senseoNode.setProperty("outOfWater").send("false");
    senseoNode.setProperty("brew").send("false");
    senseoNode.setProperty("debug").send("Machine started");
}

void UnknownState::onExit(StateId nextState)
{
    EXECUTE_IF_COMPONENT_EXIST(BuzzerComponent,playMelody("zeldaSecret"));
}

void UnknownState::onUpdate() 
{
    ledStateEnum ledState = senseoLed.getState();

    if (ledState == LED_OFF)  changeState<OffState>();
    else if (ledState == LED_SLOW) changeState<HeatingState>();
    else if (ledState == LED_FAST) changeState<NoWaterState>();
    else if (ledState == LED_ON) changeState<ReadyState>();
}