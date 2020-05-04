/*
     File: sound.c
     Author: Jacob Arsenault
     Date: May 03,2020
*/

//Dependencies
#include "sound.h"
#include <stdio.h>

//Audio Thread Function
int handle_audio(void* flags)
{
     cFlags* controlFlags= (cFlags*)flags;
     while(controlFlags->RUN)
     {
          if(updateST() != 0)      //supposed to sound when non-zero but this is my code and I like it better like this. sue me.
          {
               printf("\a");
               fflush(stdout);
          }
          updateDT();
          do {
               SDL_Delay(TIMER_DELAY);
          } while (controlFlags->PAUSE);
     }
     return 0;
}
