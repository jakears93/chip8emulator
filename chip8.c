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
#include "srcs/sound/sound.h"

//Main Program Execution Loop
int main(int argc, char* argv[])
{
     //Get command line arguements and options
     int flag;
     while((flag=getopt(argc,argv,"v:f:b:s:")) !=-1)
     {
          switch(flag)
          {
               case 'v':
                    flags.videoSizeFlag = atoi(optarg);
                    break;
               case 'f':
                    flags.frameRateFlag = atoi(optarg);
                    break;
               case 'b':
                    flags.bgColourFlag = strdup(optarg);
                    break;
               case 's':
                    flags.spriteColourFlag = strdup(optarg);
                    break;
               case '?':
                    if (optopt == 'v' || optopt == 'f')
                    {
                         printf("Option %c requires an argument.\n",optopt);
                    }
               default:
                    exit(1);
          }
     }

     //Get ROM path
     if (argv[optind] == NULL)
     {
       printf("No ROM file selected\n");
       exit(1);
     }
     else
     {
          romPath = strdup(argv[optind]);
     }

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
          frameTime = (1/(float)flags.frameRateFlag) * SECS_TO_USECS;
     }
     else if(flags.frameRateFlag == 0)
     {
          frameTime = flags.frameRateFlag;
     }
     else
     {
          frameTime = (1/(float)STD_FRAME_RATE) * SECS_TO_USECS;
     }
     if(strcmp(flags.bgColourFlag, flags.spriteColourFlag) == 0)
     {
          printf("The sprite and background colours cannot be the same.\n");
          exit(1);
     }
     //Set Background colour
     setBackgroundColour(flags.bgColourFlag);
     //Set sprite colour
     setSpriteColour(flags.spriteColourFlag);

//Initialize all modules
     cpu_init();
     graphics_init();
     draw_screen();
     input_init();
     //Seed random function
     srand(time(0));

     SDL_Thread *drawThread = NULL;
     SDL_Thread *audioThread = NULL;
     SDL_Thread *eventThread = NULL;

     //Initialize SDL timer system
     if(SDL_InitSubSystem(SDL_INIT_TIMER) < 0)
     {
          printf("Error Starting SDL Timer Subsystem: %s\n", SDL_GetError());
          exit(1);
     }

//Main emulation Loop
     while(controlFlags.RUN)
     {
          if(!controlFlags.RESET)
          {
               //Fetch next opcode, decode it and then execute
               uint16_t op = fetchInstruction();
               runCycle(op);

               //Create seperate thread for drawing to the screen
               if (drawThread == NULL)
               {
                    drawThread = SDL_CreateThread(handle_draw, "handle_draw", (void*)&controlFlags);
                    if (drawThread == NULL)
                    {
                         printf("SDL_CreateThread draw failed: %s\n", SDL_GetError());
                         exit(1);
                    }
               }
               //Create seperate thread for handling input events
               if (eventThread == NULL)
               {
                    eventThread = SDL_CreateThread(handle_event, "handle_event", (void*)&controlFlags);
                    if (drawThread == NULL)
                    {
                         printf("SDL_CreateThread event failed: %s\n", SDL_GetError());
                         exit(1);
                    }
               }
               //Create seperate thread for playing audio to the screen
               if (audioThread == NULL)
               {
                    audioThread = SDL_CreateThread(handle_audio, "handle_audio", (void*)&controlFlags);
                    if (audioThread == NULL)
                    {
                         printf("SDL_CreateThread audio failed: %s\n", SDL_GetError());
                         exit(1);
                    }
               }

               //If pause buttton is pressed, delay execution until it is pressed again.
               if(controlFlags.PAUSE)
               {
                    SDL_Delay(TIMER_DELAY);
               }
          }
          //Sleep for desired time to set frame rate
          usleep(frameTime);
     }

     //Safely shut down SDL systems and close program.
     int eventReturn = 1;
     int drawReturn = 1;
     int audioReturn = 1;

     SDL_WaitThread(eventThread, &eventReturn);
     SDL_WaitThread(audioThread, &audioReturn);
     SDL_WaitThread(drawThread, &drawReturn);

     #ifdef DEBUG
          if(eventReturn == 0)
               printf("Event Thread Closed Successfully\n");
          if(audioReturn == 0)
               printf("Audio Thread Closed Successfully\n");
          if(drawReturn == 0)
               printf("Draw Thread Closed Successfully\n");
     #endif

     SDL_QuitSubSystem(SDL_INIT_VIDEO);
     SDL_QuitSubSystem(SDL_INIT_EVENTS);
     SDL_QuitSubSystem(SDL_INIT_TIMER);
     SDL_Quit();
     return 0;
}
