/*
     File: chip8.c
     Author: Jacob Arsenault
     Date: Apr 26,2020
*/

//Dependencies
#include <time.h>
#include <unistd.h>
#include "srcs/cpu/cpu.h"
#include "srcs/graphics/graphics.h"
#include "srcs/input/input.h"
//#include "sound/sound.h"

struct FLAG{
     int videoSizeFlag;
     int frameRateFlag;
     bool stepFlag;
     char* romFlag;
} flags={0,-1,false,"none"};

static int RUN = 1;

//Draw Thread Function
int handle_draw(void *ptr)
{
     while(RUN)
     {
     //Draw to screen if drawFlag is true
          if(drawFlag)
          {
               draw_screen();
          }
          SDL_Delay(16);
     }
     return 0;
}

//Event Thread Functions
int handle_event(void *ptr)
{
     SDL_Event e;
     while (RUN)
     {
          SDL_PollEvent(&e);
          if (e.type == SDL_QUIT)
          {
               RUN = 0;
               return(0);
          }
          if (e.type == SDL_KEYDOWN)
          {
               for(int k=0; k<NUM_OF_KEYS; k++)
               {
                    if(e.key.keysym.sym == keyMap[k])
                    {
                         keyState[k] = PRESSED;
                    }
               }
          }
          else if (e.type == SDL_KEYUP)
          {
               for(int k=0; k<NUM_OF_KEYS; k++)
               {
                    if(e.key.keysym.sym == keyMap[k])
                    {
                         keyState[k] = RELEASED;
                    }
               }
          }
          SDL_Delay(8);
     }
     return 0;
}

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
     #ifdef DEBUG
          printf("Video Size Flag = %d\n", flags.videoSizeFlag);
          printf("Frame Rate Flag = %d\n", flags.frameRateFlag);
          printf("Step Flag = %s\n", flags.stepFlag ? "true" : "false");
          printf("Rom Flag = %s\n\n", flags.romFlag);
     #endif
     //Process arg flags

     //Adjust window scale
     if (flags.videoSizeFlag > 0 && flags.videoSizeFlag <= 30)
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
          frameTime = STD_FRAME_RATE*SECS_TO_USECS;
     }

     //Set rom path
     if(strcmp(flags.romFlag, "none") != 0)
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
     draw_screen();
     //sound_init();
     input_init();
     srand(time(0));

     SDL_Thread *drawThread = NULL;
     SDL_Thread *eventThread = NULL;


     if(SDL_InitSubSystem(SDL_INIT_TIMER) < 0)
     {
          printf("Error Starting SDL Timer Subsystem: %s\n", SDL_GetError());
          exit(1);
     }

     //Main emulation Loop

     while(RUN)
     {
          //Require a key press to execute cycle if stepFlag is set
          if(flags.stepFlag)
          {
               getchar();
          }

          //Fetch next opcode, decode it and then execute
          uint16_t op = fetchInstruction();
          runCycle(op);

          if (drawThread == NULL)
          {
               drawThread = SDL_CreateThread(handle_draw, "handle_draw", (void*)NULL);
               if (drawThread == NULL)
               {
                    printf("SDL_CreateThread draw failed: %s\n", SDL_GetError());
                    exit(1);
               }
          }

          if (eventThread == NULL)
          {
               eventThread = SDL_CreateThread(handle_event, "handle_event", (void*)NULL);
               if (drawThread == NULL)
               {
                    printf("SDL_CreateThread event failed: %s\n", SDL_GetError());
                    exit(1);
               }
          }



          //Decrement Timers
          if(REG.DT == 0)
          {
               REG.DT = 0xFF;
          }
          else
          {
               REG.DT = REG.DT-1;
          }
          if(REG.ST == 0)
          {
               REG.ST = 0xFF;
          }
          else
          {
               REG.ST = REG.ST-1;
          }

          //Sleep for desired time to set frame rate
          usleep(frameTime);
	     //TODO figure out why sleep function is causing jittering of image
     }
     SDL_WaitThread(drawThread, NULL);
     SDL_QuitSubSystem(SDL_INIT_VIDEO);
     SDL_QuitSubSystem(SDL_INIT_EVENTS);
     SDL_QuitSubSystem(SDL_INIT_TIMER);
     SDL_Quit();

     return 0;
}
