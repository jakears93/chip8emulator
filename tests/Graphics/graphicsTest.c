/*
     File: graphicsTest.c
     Author: Jacob Arsenault
     Date: Apr 27,2020
*/

//Dependencies
#include "../../srcs/graphics/graphics.h"
#include "../../srcs/cpu/cpu.h"
#include <unistd.h>


//TESTS CREATION OF SDL WINDOW AND DRAWING OF BLANK SCREEN
int main()
{
     int toggle = 0;
     windowScale = 10;
     cpu_init();
     graphics_init();

     while(1)
     {
          SDL_Event e;
          while (SDL_PollEvent(&e)) {
              if (e.type == SDL_QUIT) exit(0);
          }

          for(int row=0; row<SCREEN_HEIGHT; row++)
          {
               for(int col=0; col<SCREEN_WIDTH; col++)
               {
                    display[col][row] = 0;
                    if((col>(SCREEN_WIDTH/2))&&toggle)
                    display[col][row] = 1;
               }
          }
          toggle ^= 1;
          draw_screen();
          usleep(1*1000*1000);
     }
     return 0;
}
