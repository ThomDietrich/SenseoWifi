#include "SenseoState.h"
#include <Homie.h>

#include "../Components/ControlComponent.h"
#include "../Components/SenseoLedComponent.h"
#include "../Components/CupComponent.h"

void SenseoState::onInitialized()
{
    commandComponent = getComponent<CommandComponent>();
    controlComponent = getComponent<ControlComponent>();
    assert(commandComponent != nullptr);
    assert(controlComponent != nullptr);

    // the CupComponent is optional
    cupComponent = getComponent<CupComponent>();
}

bool SenseoState::hasOffCommands() const
{
    if (commandComponent->hasPendingCommands(CommandComponent::TurnOff)) return true;
    else if (commandComponent->hasPendingCommands(CommandComponent::TurnOffAfterBrewing))
    {
        if (commandComponent->hasPendingCommands(CommandComponent::Brew1Cup | CommandComponent::Brew2Cup)) return false;
        else if (commandComponent->hasProcessedCommands(CommandComponent::Brew1Cup | CommandComponent::Brew2Cup)) return false;
        else return true;
    }

    return false;
}

void SenseoState::processOffCommands()
{
    controlComponent->pressPowerButton();
    processCommands(CommandComponent::TurnOff | CommandComponent::TurnOffAfterBrewing);

    //This should improved perceived reactivity 
    EXECUTE_IF_COMPONENT_EXIST(SenseoLedComponent,turnOff());
}

void SenseoState::processCommands(CommandComponent::CommandBitFields commands)
{
    if ((commands & commandComponent->getPendingCommands()) != 0)
    {
        Homie.getLogger() << getStateName() << ": Processing commands " << commandComponent->getCommandsAsString(commands & commandComponent->getPendingCommands()) << endl;
    }
    commandComponent->proccessCommands(commands); 
}

void SenseoState::clearCommands(CommandComponent::CommandBitFields commands) 
{ 
    if ((commands & commandComponent->getPendingCommands()) != 0)
    {
        Homie.getLogger() << getStateName() << ": Clearing pending commands " << commandComponent->getCommandsAsString(commands & commandComponent->getPendingCommands()) << endl;
    }
    if ((commands & commandComponent->getProcessedCommands()) != 0)
    {
        Homie.getLogger() << getStateName() << ": Clearing processed commands " << commandComponent->getCommandsAsString(commands & commandComponent->getProcessedCommands()) << endl;
    }
    commandComponent->clearCommands(commands); 
}