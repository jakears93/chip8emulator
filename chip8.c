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

struct FLAG{
     int videoSizeFlag;
     int frameRateFlag;
     bool stepFlag;
     char* romFlag;
} flags={0,-1,false,""};

//Function Definitions
int main(int argc, char* argv[])
{
     //TODO
     //Set up config files to set default flags

     //Get cmd args
     int flag;
     while((flag=getopt(argc,argv,":v:f:sr:")) !=-1)
     {
          switch(flag)
          {
               case 'v':
                    flags.videoSizeFlag = atoi(optarg);
                    break;
               case 'f':
                    flags.frameRateFlag = atoi(optarg);
                    break;
               case 's':
                    flags.stepFlag = true;
                    break;
               case 'r':
                    flags.romFlag = strdup(optarg);
          }
     }

     printf("Video Size Flag = %d\n", flags.videoSizeFlag);
     printf("Frame Rate Flag = %d\n", flags.frameRateFlag);
     printf("Step Flag = %s\n", flags.stepFlag ? "true" : "false");
     printf("Rom Flag = %s\n", flags.romFlag);

     //Process arg flags

     //Adjust window scale
     if (flags.videoSizeFlag > 0 && flags.videoSizeFlag <= 20)
     {
               windowScale = flags.videoSizeFlag;
     }
     else
     {
          windowScale = DEFAULT_SCALE;
     }

     //Adjust frame timing
     if(flags.frameRateFlag > 0)
     {
          frameTime = (1/(float)flags.frameRateFlag)*SECS_TO_USECS;
     }
     else if(flags.frameRateFlag == 0)
     {
          frameTime = flags.frameRateFlag;
     }
     else
     {
          frameTime = STD_FRAME_TIME;
     }

     //Set rom path
     if(strcmp(flags.romFlag, "") != 0)
     {
          romPath = strdup(flags.romFlag);
     }
     else if(flags.videoSizeFlag==0 && flags.frameRateFlag==-1 && flags.stepFlag==false && strcmp(flags.romFlag, "")==0)
     {
          romPath = argv[1];
     }
     else
     {
          printf("To use, run command:\n\t./chip8 <romPath>\n\tor:\n\t./chip8 -r <romPath> -optionalFlags\n");
          exit(1);
     }

     //Initialize all modules
     cpu_init();
     graphics_init();
     //sound_init();
     //input_init();

     //Main emulation Loop
     while(RUN)
     {
          //Require a key press to execute cycle if stepFlag is set
          if(flags.stepFlag)
          {
               getchar();
          }

          SDL_Event e;
          while (SDL_PollEvent(&e)) {
              if (e.type == SDL_QUIT) return(0);
          }

          //Fetch next opcode, decode it and then execute
          uint16_t op = fetchInstruction();
          runCycle(op);

          //Draw to screen if drawFlag is true
          if(drawFlag == true)
          {
               printf("\tDrawing To Screen\n");
               draw_screen();
          }

          //Decrement Timers
          REG.DT--;
          REG.ST--;

          //Sleep for desired time to set frame rate
          usleep(frameTime);
     }

     return 0;
}
