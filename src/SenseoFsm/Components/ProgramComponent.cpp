
#include "programComponent.h"
#include <Homie.h>

ProgramComponent::ProgramComponent(HomieNode & node) 
: senseoNode(node) 
{
    clearProgram();
}

void ProgramComponent::clearProgram()
{
    hasProgram = false;
    senseoNode.setProperty("hasProgram").send("false");        
    senseoNode.setProperty("programContext").send("");
}
        
void ProgramComponent::requestProgram(Program program,bool powerPressed)
{
    DynamicJsonDocument jsonContext(1024);
    if (program == oneCup) jsonContext["cup_size"] = "1";
    else if (program == twoCup) jsonContext["cup_size"] = "2";

    if (powerPressed) jsonContext["power_pressed"] = "true";
    else jsonContext["power_pressed"] = "false";

    String jsonString;
    serializeJson(jsonContext,jsonString);

    hasProgram = true;
    senseoNode.setProperty("hasProgram").send("true");        
    senseoNode.setProperty("programContext").send(jsonString);
}