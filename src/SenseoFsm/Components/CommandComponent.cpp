#include "CommandComponent.h"
#include <Homie.h>

const char * CommandComponent::commandToString(Command command)
{ 
    auto iter = commandName.find(command);
    if (iter != commandName.end()) return iter->second;
    return nullptr;
}

String CommandComponent::getCommandsAsString(CommandBitFields commands)
{
    String commandString;
    for(const auto & it: commandName) 
    {
        if (hasCommand(commands,it.first))
        {
            if (commandString.isEmpty() == false) commandString += " | ";
            commandString += it.second;
        }
    }
    return commandString;
}

void CommandComponent::proccessCommands(CommandBitFields commands) 
{
    CommandBitFields pending = commands & pendingCommands;
    ProcessedCommands |= pending; //add command to the Processed command
    pendingCommands &= ~pending; //remove command from the pending command
    updateSenseoNode();
}

void CommandComponent::clearCommands(CommandBitFields commandsToClear) 
{ 
    pendingCommands &= ~commandsToClear; 
    ProcessedCommands &= ~commandsToClear; 
    updateSenseoNode(); 
}

void CommandComponent::sendCommands(CommandBitFields newCommands)
 { 
    pendingCommands = newCommands; 
    ProcessedCommands = 0;
    updateSenseoNode(); 
}

void CommandComponent::updateSenseoNode()
{
    senseoNode.setProperty("pendingCommands").send(getCommandsAsString(pendingCommands));
    senseoNode.setProperty("processedCommands").send(getCommandsAsString(ProcessedCommands));
}