/*
     File: graphicsTest.c
     Author: Jacob Arsenault
     Date: Apr 27,2020
*/

//Dependencies
#include "../../srcs/graphics/graphics.h"
#include <unistd.h>


//TESTS CREATION OF SDL WINDOW AND DRAWING OF BLANK SCREEN
int main()
{
     graphics_init();

     while(1)
     {
          SDL_Event e;
          while (SDL_PollEvent(&e)) {
              if (e.type == SDL_QUIT) exit(0);
         }
          draw_screen();
     }
     return 0;
}
