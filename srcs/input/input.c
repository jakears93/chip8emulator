/*
     File: input.c
     Author: Jacob Arsenault
     Date: Apr 26,2020
*/

//Dependencies
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "input.h"

//Function Definitions
int input_init(void)         //Initialize the inputs based on config file, set all key states to released
{
     if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
     {
          printf("Error Starting SDL Events Subsystem: %s\n", SDL_GetError());
          exit(1);
     }

     KEYS[0] = KEY_1;
     KEYS[1] = KEY_2;
     KEYS[2] = KEY_3;
     KEYS[3] = KEY_C;
     KEYS[4] = KEY_4;
     KEYS[5] = KEY_5;
     KEYS[6] = KEY_6;
     KEYS[7] = KEY_D;
     KEYS[8] = KEY_7;
     KEYS[9] = KEY_8;
     KEYS[10] = KEY_9;
     KEYS[11] = KEY_E;
     KEYS[12] = KEY_A;
     KEYS[13] = KEY_0;
     KEYS[14] = KEY_B;
     KEYS[15] = KEY_F;

     FILE *input_file;
     char temp;
     input_file = fopen(INPUT_CONFIG_FILE, "r");
     if(input_file == NULL)
     {
          printf("Input File could not be opened\n");
          return 1;
     }

     for (int i =0; i<NUM_OF_KEYS; i++){
          if((temp = fgetc(input_file)) != EOF)
               keyMap[i] = temp;
          else
          {
               printf("Input File format is incorrect\n");
               return 1;
          }
          keyState[i] = RELEASED;
     }

     fclose(input_file);
     return 0;
}

int getKeyPress(void)              //Wait and read a key press, return the key that was press
{
    for(int k=0; k<NUM_OF_KEYS; k++)
    {
         if(keyState[k] == PRESSED)
	 {
 	      return KEYS[k];
	 }
    }
    return -1;
}

void toggle_key(int key)          //Toggle key state
{
     keyState[key] ^= 1;
}

void debug_printKeys()             //Print key values and states to console for debugging.
{
     printf("PRINTING KEY VALUES AND STATES\n");
     for(int i=0;i<NUM_OF_KEYS;i++)
     {
          printf("Key %X\tMapping: %c\tState: %d\n",KEYS[i],keyMap[i],keyState[i]);
     }
     printf("END OF PRINT KEYS\n\n");
}
