/*
     File: inputTest.c
     Author: Jacob Arsenault
     Date: Apr 26,2020
*/

//Dependencies
#include "../../srcs/input/input.h"
#include <stdlib.h>
#include <stdio.h>


//TESTS THE INIT AND TOGGLE FUNCTIONS FROM input.c
int main()
{
     char key;
     if (input_init())
     {
          printf("INIT FAILED");
          return 1;
     }
     else
     {
          while(1)
          {
               printf("Press a key (! to exit): ");
               key = getchar();
               if(key == '!')
                    return 0;
               getchar();          //Clear newline character
               toggle_key(key);
               debug_printKeys();
          }
     }

     return 0;
}
