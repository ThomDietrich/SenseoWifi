#include "OffState.h"
#include <Homie.h>
#include "LedObserver/ILedObserver.h"
#include "HeatingState.h"
#include "NoWaterState.h"
#include "ReadyState.h"
#include "UnknownState.h"
#include "../Components/BuzzerComponent.h"
#include "../Components/SenseoLedComponent.h"
#include "../Components/ControlComponent.h"
#include "../Components/ProgramComponent.h"

void OffState::onInitialized()
{
    SenseoState::onInitialized();
    programComponent = getComponent<ProgramComponent>();
}

void OffState::onEnter(StateId previousState) 
{
    clearCommands(CommandComponent::All);
    EXECUTE_IF_COMPONENT_EXIST(SenseoLedComponent,turnOff());
    senseoNode.setProperty("brewedSize").send("0");
    senseoNode.setProperty("power").send("false");
}

void OffState::onUpdate() 
{
    ledStateEnum ledState = senseoLed.getState();

    if (ledState == LED_SLOW) changeState<HeatingState>();
    else if (ledState == LED_FAST) changeState<NoWaterState>();
    else if (ledState == LED_ON) changeState<ReadyState>();
    else if (hasPendingCommands(CommandComponent::TurnOn))
    {
        controlComponent->pressPowerButton();
        processCommands(CommandComponent::TurnOn);
        //This should improved perceived reactivity 
        EXECUTE_IF_COMPONENT_EXIST(SenseoLedComponent,turnOn());
    }
    else if (programComponent != nullptr && ! hasProcessedCommands(CommandComponent::TurnOn))
    {
        if (programComponent->hasAnyProgram())
        {
            EXECUTE_IF_COMPONENT_EXIST(SenseoLedComponent,blink(500,30000));
        }
        else
        {
            EXECUTE_IF_COMPONENT_EXIST(SenseoLedComponent,turnOff());   
        }
    }

}

void OffState::onExit(StateId nextState) 
{
    if (nextState != UnknownState::s_StateId) clearCommands(CommandComponent::TurnOn);
    senseoNode.setProperty("power").send("true");
    senseoNode.setProperty("outOfWater").send("false");
    senseoNode.setProperty("brew").send("false");
}