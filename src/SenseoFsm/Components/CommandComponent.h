#pragma once

#include "ModularFsm/FsmComponent.h"
#include <map>
#include <Arduino.h>

class HomieNode;


class CommandComponent : public  FsmComponent<CommandComponent>
{
    public:
        CommandComponent(HomieNode & node) : senseoNode(node) {}
        using CommandBitFields = unsigned char;
        enum Command : CommandBitFields
        {
            None = 0,
            TurnOn = 1,
            TurnOff = 2,
            Brew1Cup = 4,
            Brew2Cup = 8,
            TurnOffAfterBrewing = 16,
            All = 0xff,
        };

        void sendCommands(CommandBitFields newCommands);
        void proccessCommands(CommandBitFields newCommands);
        void clearCommands(CommandBitFields commands);

        //will return true if any of the commands is present in the pending command
        bool hasPendingCommands(CommandBitFields command) const { return (pendingCommands & command) != 0; };
        //will return true if any of the commands is present in the Processed command
        bool hasProcessedCommands(CommandBitFields command) const { return (ProcessedCommands & command) != 0; };
        
        CommandBitFields getPendingCommands() const { return pendingCommands; }
        CommandBitFields getProcessedCommands() const { return ProcessedCommands; }

        String getPendingCommandsAsString() const { return getCommandsAsString(pendingCommands); };
        String getProcessedCommandsAsString() const { return getCommandsAsString(ProcessedCommands); };
        
        static const char * commandToString(Command command);        
        static String getCommandsAsString(CommandBitFields commands);
        static bool hasCommand(CommandBitFields commands, Command command) { return (commands & command) == command; }
        
    private:
        void updateSenseoNode();
        CommandBitFields ProcessedCommands = 0;
        CommandBitFields pendingCommands = 0;
        HomieNode & senseoNode;

        static inline const std::map<Command,const char *> commandName = {{TurnOn,"TurnOn"},{TurnOff,"TurnOff"},{Brew1Cup,"Brew1Cup"},{Brew2Cup,"Brew2Cup"},{TurnOffAfterBrewing,"TurnOffAfterBrewing"}};
};