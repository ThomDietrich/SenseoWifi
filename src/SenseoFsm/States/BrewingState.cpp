#include "BrewingState.h"
#include <Homie.h>
#include "constants.h"
#include "LedObserver/ILedObserver.h"
#include "NoWaterState.h"
#include "ReadyState.h"
#include "OffState.h"
#include "HeatingState.h"
#include "constants.h"
#include "../Components/CupComponent.h"
#include "../Components/SenseoLedComponent.h"
#include "../Components/ControlComponent.h"
#include "../Components/BuzzerComponent.h"

void BrewingState::onEnter(StateId previousState) 
{
    EXECUTE_IF_COMPONENT_EXIST(SenseoLedComponent,blink(2000,500));

    if (cupComponent != nullptr) 
    {
        // I'm not sure if that can really happen, I just let it here because if was here before me
        if (cupComponent->isAvailable()) cupComponent->setFilling();
        else senseoNode.setProperty("debug").send("cup: Brewing without detected cup, will not report the filling->full process.");
    }
    senseoNode.setProperty("brew").send("true");
    senseoNode.setProperty("brewedSize").send("0");
    brewingCancel = false;
}
void BrewingState::onUpdate() 
{
    ledStateEnum ledState = senseoLed.getState();

    // Stop Brewing if the cup is gone
    if (cupComponent != nullptr && !cupComponent->isAvailable() && !brewingCancel) 
    {
        brewingCancel = true;

        Homie.getLogger() << "The cup is gone, Brewing cancel." << endl;
        senseoNode.setProperty("debug").send("The cup is gone, Brewing cancel.");
        clearCommands(CommandComponent::All);
        commandComponent->sendCommands(CommandComponent::TurnOff);
    }

    if (ledState == LED_OFF)  changeState<OffState>();
    else if (hasOffCommands()) processOffCommands();
    else if (ledState == LED_FAST) changeState<NoWaterState>();
    else if (ledState == LED_ON && getTimeInState() > 5000) changeState<ReadyState>();
    else if (hasProcessedCommands(CommandComponent::Brew1Cup) && getTimeInState() > Brew1CupMillies) changeState<HeatingState>();
    else if (hasProcessedCommands(CommandComponent::Brew2Cup) && getTimeInState() > Brew2CupMillies) changeState<HeatingState>();
}

void BrewingState::onExit(StateId nextState) 
{
    EXECUTE_IF_COMPONENT_EXIST(BuzzerComponent,playMelody("cupReady"));
    senseoNode.setProperty("brew").send("false");

    int brewedSize = 0;    
    int brewedSeconds = (getTimeInState()+ 500) / 1000;    
    
    if (hasProcessedCommands(CommandComponent::Brew1Cup)) brewedSize = 1;
    else if (hasProcessedCommands(CommandComponent::Brew2Cup)) brewedSize = 2;
    else if (brewedSeconds > 10) 
    {
        // Determine brewed cup size based on time in brewing state
        // 0---------------------|-----+-----|-----+-----|-------100
        int tolerance = (BrewHeat2CupSeconds - BrewHeat1CupSeconds) / 2;

        if (nextState == ReadyState::s_StateId) 
        {
            if (abs(brewedSeconds - BrewHeat1CupSeconds) < tolerance) brewedSize = 1;
            else if (abs(brewedSeconds - BrewHeat2CupSeconds) < tolerance) brewedSize = 2;
        }

        tolerance = (Brew2CupSeconds - Brew1CupSeconds) / 2;
        if (nextState == NoWaterState::s_StateId || nextState == OffState::s_StateId) 
        {
            if (abs(brewedSeconds - Brew1CupSeconds) < tolerance) brewedSize = 1;
            else if (abs(brewedSeconds - Brew2CupSeconds) < tolerance) brewedSize = 2;
        }
    }

    senseoNode.setProperty("brewedSize").send(String(brewedSize));
    if (brewedSize == 0) 
    {
        senseoNode.setProperty("debug").send("brew: Unexpected time in SENSEO_BREWING state. Please adapt timings.");
    }
    if (cupComponent != nullptr) 
    {
        if (cupComponent->isFilling()) cupComponent->setFull();
    }   

    clearCommands(CommandComponent::Brew1Cup | CommandComponent::Brew2Cup);
}