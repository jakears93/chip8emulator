/*
     File: chip8.c
     Author: Jacob Arsenault
     Date: Apr 26,2020
*/

//Dependencies
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "srcs/cpu/cpu.h"
#include "srcs/graphics/graphics.h"
//#include "sound/sound.h"
#include "srcs/input/input.h"

//Function Definitions
int main(int argc, char* argv[])
{
     bool runFlag = true;

     //Set up config files
     //TODO

     //Initialize all modules
     cpu_init();
     if(argc > 1)
     {
          loadRom(argv[1]);
     }
     else
     {
          printf("To use, run command ./chip8 <romPath> <Step> <scale>\n");
          exit(1);
     }

     if (argv[2] != NULL)
     {
          if(atoi(argv[2]) <=20 && atoi(argv[2]) >=1)
               {
                    windowScale = atoi(argv[2]);
               }
               else
               {
                    windowScale = 10;
               }
     }
     else
          windowScale=10;



     graphics_init();
     //sound_init();
     //input_init();

     //Main emulation Loop
     while(runFlag)
     {
          SDL_Event e;
          while (SDL_PollEvent(&e)) {
              if (e.type == SDL_QUIT) exit(0);
          }
          uint16_t op = fetchInstruction();
          runCycle(op);
          if(drawFlag == true)
          {
               printf("\tDrawing To Screen\n");
               draw_screen();
          }
          REG.DT--;
          usleep(17*1000);
     }

     return 0;
}
