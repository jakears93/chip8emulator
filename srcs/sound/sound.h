/*
     File: sound.h
     Author: Jacob Arsenault
     Date: May 03,2020
*/
#ifndef SOUND_H
#define SOUND_H

//Dependencies
#include "SDL2/SDL.h"
#include "../cpu/cpu.h"

//Constants

//Global Variables

//Function Declarations
int handle_audio(void* flags);          //Callback function for audio thread

#endif
