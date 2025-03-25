#include "BuzzerComponent.h"
#include <ezBuzzer.h>
#include "MusicLibrary.h"
#include <Homie.h>
#include "pins.h"

#define REST 0

const char * BuzzerComponent::getValidTunes()
{
    static String s_validTunes; //It need to be static since we return a const char *
    if (s_validTunes.isEmpty())
    {
        for (const auto & item : musicLibrary)
        {
            if (!s_validTunes.isEmpty()) s_validTunes += ",";
            s_validTunes += item.first;
        }
    }

    return s_validTunes.c_str();
}

bool BuzzerComponent::playMelody(const String & tune) 
{
    Homie.getLogger() << "Playing " << tune << endl;
    auto iter = musicLibrary.find(tune);
    if (iter != musicLibrary.end())
    {
        assert(iter->second.notes.size() == iter->second.durations.size());
        // the buffer should be const in the library
        myBuzzer.playMelody(const_cast<int*>(iter->second.notes.data()), const_cast<int*>(iter->second.durations.data()), iter->second.durations.size());
        return true;
    }
    else
    {
        Homie.getLogger() << "Tune not found. Valid tunes are: " <<  getValidTunes() << endl;
        return false;
    }
}

void BuzzerComponent::update() {
    myBuzzer.loop();
}