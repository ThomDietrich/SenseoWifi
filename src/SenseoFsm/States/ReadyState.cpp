#include "ReadyState.h"
#include "LedObserver/ILedObserver.h"
#include "BrewingState.h"
#include "NoWaterState.h"
#include "OffState.h"
#include "homie.h"
#include "../Components/BuzzerComponent.h"
#include "../Components/SenseoLedComponent.h"
#include "../Components/CommandComponent.h"
#include "../Components/ControlComponent.h"
#include "../Components/CupComponent.h"

void ReadyState::onEnter(StateId previousState) 
{
    EXECUTE_IF_COMPONENT_EXIST(SenseoLedComponent,turnOn());
    waitingForACup = false;
}

void ReadyState::processBrewingCommand(CommandComponent::Command command)
{
    assert(command == CommandComponent::Brew1Cup || command == CommandComponent::Brew2Cup);

    // Let's check if we have a cup in place
    if (cupComponent == nullptr || (cupComponent->isAvailable() && !cupComponent->isFull()))
    {
        if (command == CommandComponent::Brew1Cup) controlComponent->pressLeftButton();
        else if (command == CommandComponent::Brew2Cup) controlComponent->pressRightButton();
        else assert(!"Invalid command");

        processCommands(command);
        changeState<BrewingState>();  
    }   
    else if (!waitingForACup)
    {
        Homie.getLogger() << "Waiting for a cup" << endl;
        senseoNode.setProperty("debug").send("Ready: waiting for cup"); 
        waitingForACup = true;
    }
}

void ReadyState::onUpdate() 
{
    ledStateEnum ledState = senseoLed.getState();

    if (ledState == LED_OFF)  changeState<OffState>();
    else if (hasOffCommands()) processOffCommands();
    else if (ledState == LED_SLOW && useCustomizableButtonAddon) changeState<BrewingState>(); //With the customizable button addon, it's impossibble to reach brewing state without going through a command
    else if (ledState == LED_FAST) changeState<NoWaterState>();
    else if (hasPendingCommands(CommandComponent::Brew1Cup)) processBrewingCommand(CommandComponent::Brew1Cup);
    else if (hasPendingCommands(CommandComponent::Brew2Cup)) processBrewingCommand(CommandComponent::Brew2Cup);
}