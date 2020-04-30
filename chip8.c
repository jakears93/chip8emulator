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

//Struct to hold flags for cmd options/args
struct FLAG{
     int videoSizeFlag;
     float frameRateFlag;
     char* bgColourFlag;
     char* spriteColourFlag;
} flags={0,-1, "black", "white"};

//Emulation Control Flags
bool RESET = 0;
bool RUN = 1;
bool PAUSE = 0;
bool PAUSE_SET = 0;
bool SPEED_SET = 0;

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
          do
          {
               SDL_Delay(8);
          } while(PAUSE);
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
               if(PAUSE)
               {
                    PAUSE = 0;
               }
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
               if(e.key.keysym.sym == SDLK_BACKSPACE)
               {
                    if(PAUSE_SET == 0)
                    {
                         PAUSE = 1;
                    }
                    else
                    {
                         PAUSE = 0;
                    }
               }
               if(e.key.keysym.sym == SDLK_KP_PLUS)
               {
                         flags.frameRateFlag += 100;
                         frameTime = (1/(float)flags.frameRateFlag)*SECS_TO_USECS;
               }
               else if(e.key.keysym.sym == SDLK_KP_MINUS)
               {
                         flags.frameRateFlag -= 100;
                         if(flags.frameRateFlag < MIN_FRAME_RATE)
                         {
                              flags.frameRateFlag = MIN_FRAME_RATE;
                         }
                         frameTime = (1/(float)flags.frameRateFlag)*SECS_TO_USECS;
               }
               if(e.key.keysym.sym == SDLK_F1)
               {
                    if(RESET == 0)
                    {
                         rom_reset();
                         RESET = 1;
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
               if(e.key.keysym.sym == SDLK_BACKSPACE)
               {
                    if(PAUSE == 1)
                    {
                         PAUSE_SET = 1;
                    }
                    else
                    {
                         PAUSE_SET = 0;
                    }
               }
               if(e.key.keysym.sym == SDLK_F1)
               {
                    if(RESET == 1)
                    {
                         RESET = 0;
                    }
               }
          }
          SDL_Delay(8);
     }
     return 0;
}

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
     //sound_init();
     input_init();
     srand(time(0));

     SDL_Thread *drawThread = NULL;
     SDL_Thread *eventThread = NULL;

     //Initialize SDL timer system
     if(SDL_InitSubSystem(SDL_INIT_TIMER) < 0)
     {
          printf("Error Starting SDL Timer Subsystem: %s\n", SDL_GetError());
          exit(1);
     }

//Main emulation Loop
     while(RUN)
     {
          if(!RESET)
          {
               //Fetch next opcode, decode it and then execute
               uint16_t op = fetchInstruction();
               runCycle(op);

               //Create seperate thread for drawing to the screen
               if (drawThread == NULL)
               {
                    drawThread = SDL_CreateThread(handle_draw, "handle_draw", (void*)NULL);
                    if (drawThread == NULL)
                    {
                         printf("SDL_CreateThread draw failed: %s\n", SDL_GetError());
                         exit(1);
                    }
               }
               //Creat seperate thread for handling input events
               if (eventThread == NULL)
               {
                    eventThread = SDL_CreateThread(handle_event, "handle_event", (void*)NULL);
                    if (drawThread == NULL)
                    {
                         printf("SDL_CreateThread event failed: %s\n", SDL_GetError());
                         exit(1);
                    }
               }

               //Decrement Delay and Sound Timers
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

               //If pause buttton is pressed, delay execution until it is pressed again.
               if(PAUSE)
               {
                    SDL_Delay(16);
               }
          }
          //Sleep for desired time to set frame rate
          usleep(frameTime);
     }

     //Safely shut down SDL systems and close program.
     SDL_WaitThread(drawThread, NULL);
     SDL_QuitSubSystem(SDL_INIT_VIDEO);
     SDL_QuitSubSystem(SDL_INIT_EVENTS);
     SDL_QuitSubSystem(SDL_INIT_TIMER);
     SDL_Quit();
     return 0;
}
