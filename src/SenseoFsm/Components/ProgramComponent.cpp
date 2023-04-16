
#include "programComponent.h"
#include <Homie.h>

void ProgramComponent::clearProgram(Program program)
{
    if (program == all || program == oneCup)
    {
        programOneCup = false;
        senseoNode.setProperty("program1Cup").send("false");
    }
    if (program == all || program == twoCup)
    {
        programTwoCup = false;
        senseoNode.setProperty("program2Cup").send("false");
    }
}
        
void ProgramComponent::requestProgram(Program program)
{
    if (program == oneCup)
    {
        programOneCup = true;
        programTwoCup = false;
        senseoNode.setProperty("program1Cup").send("true");        
        senseoNode.setProperty("program2Cup").send("false");
    }
    if (program == twoCup)
    {
        programOneCup = false;
        programTwoCup = true;
        senseoNode.setProperty("program1Cup").send("false");
        senseoNode.setProperty("program2Cup").send("true");
    }
}