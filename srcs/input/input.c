/*
     File: input.c
     Author: Jacob Arsenault
     Date: Apr 26,2020
*/

//Dependencies
#include <stdio.h>
#include <stdlib.h>
#include "input.h"

//Function Definitions
int input_init(void)         //Initialize the inputs based on config file, set all key states to released
{
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
     //TODO
     //Uses SDL
}

void toggle_key(char key)          //Toggle key state
{
     for(int i=0; i<NUM_OF_KEYS; i++)
     {
          if(key == keyMap[i])
          {
               keyState[i] ^= 1;
               break;
          }
     }
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
