#pragma once
#include <map>
#include <ezBuzzer.h>
#include <vector>
#define REST 0

//most of the melody came from : https://github.com/hibit-dev/buzzer

struct BuzzerMelody
{
    std::vector<int> notes;
    std::vector<int> durations;
};

static inline const std::map<String, const BuzzerMelody> musicLibrary = {
    { "beep", {
        { NOTE_C7 },
        { 2 }
    }},
    { "melody2", {
        { NOTE_E5, NOTE_E5, NOTE_F5, NOTE_C5 } ,
        { 4, 8, 8, 2 } 
    }},
    { "melody3", {
        { NOTE_C4, NOTE_C5 } ,
        { 4, 8 } 
    }},
    { "got", {
        { 
            NOTE_E4, NOTE_A4,NOTE_C5,NOTE_D5,NOTE_E5,
            NOTE_A4,NOTE_C5,NOTE_D5,NOTE_B4 
        } ,
        { 
            2,2, 8,8,2,
            2,8,8,1
        } 
    }},
    { "zeldaHeart", {
        { 
            NOTE_GS3, NOTE_C3,NOTE_D4,
            NOTE_F4,NOTE_GS4,NOTE_C5,NOTE_D5,
            NOTE_F5,NOTE_GS5,NOTE_C6,NOTE_D6,
            NOTE_D6,NOTE_D6
        } ,
        { 
            4, 8,8,
            8,8,8,8,
            8,8,8,8,
            4,2
        } 
    }},
    { "zeldaSecret", {
        { 
            NOTE_G5, NOTE_FS5,NOTE_DS5,NOTE_A4,
            NOTE_GS4,NOTE_E5,NOTE_GS5,NOTE_C6
        } ,
        { 
            9,9,9,9,
            9,9,9,9
        } 
    }},
    { "acnh", {
        { 
            NOTE_C5, NOTE_A4,NOTE_F4,NOTE_D5,
            NOTE_C5,NOTE_A4,NOTE_F4,NOTE_F4,NOTE_D4,NOTE_G4
        } ,
        { 
            4,4,4,4,
            2,4,4,4,4,2
        } 
    }},
    { "cupReady", { 
        { NOTE_A4, NOTE_AS4, NOTE_B4, NOTE_C5 }, 
        { 8, 8, 8,2 }
    }},
    
    { "potc", { 
        { 
            NOTE_E4, NOTE_G4, NOTE_A4, NOTE_A4, REST,
            NOTE_A4, NOTE_B4, NOTE_C5, NOTE_C5, REST,
            NOTE_C5, NOTE_D5, NOTE_B4, NOTE_B4, REST,
            NOTE_A4, NOTE_G4, NOTE_A4,
        }, 
        { 
            8, 8, 4, 8, 8,
            8, 8, 4, 8, 8,
            8, 8, 4, 8, 8,
            8, 8, 4,
        }
    }},
    { "harry", {
        {
            REST, NOTE_D4,
            NOTE_G4, NOTE_AS4, NOTE_A4,
            NOTE_G4, NOTE_D5,
            NOTE_C5, 
            NOTE_A4,
            NOTE_G4, NOTE_AS4, NOTE_A4,
            NOTE_F4, NOTE_GS4,
            NOTE_D4,
        },
        {
            2, 4,
            4, 8, 4,
            2, 4,
            2, 
            2,
            4, 8, 4,
            2, 4,
            1,
        }
    }},
    { "mario", {
        {
            NOTE_E5, NOTE_E5, REST, NOTE_E5, REST, NOTE_C5, NOTE_E5,
            NOTE_G5, REST, NOTE_G4, //REST, 
        },
        {
            8, 8, 8, 8, 8, 8, 8,
            4, 4, 8, //4, 
        }
    }},
    { "sw", {
        {
            NOTE_AS4, NOTE_AS4, NOTE_AS4,
            NOTE_F5, NOTE_C6,
            NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
            NOTE_AS5, NOTE_A5, NOTE_G5, NOTE_F6, NOTE_C6,
            NOTE_AS5, NOTE_A5, NOTE_AS5, NOTE_G5,
        },
        {
            8, 8, 8,
            2, 2,
            8, 8, 8, 2, 4,
            8, 8, 8, 2, 4,
            8, 8, 8, 2, 
        }
    }},
};