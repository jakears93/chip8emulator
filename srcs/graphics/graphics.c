/*
     File: graphics.c
     Author: Jacob Arsenault
     Date: Apr 26,2020
*/

//Dependencies
#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include "../cpu/cpu.h"

//Function Definitions
void graphics_init(void)                //Set pixels to 0, load font sprites to ram
{
     //Set pixels to 0
     for(int row=0; row<SCREEN_HEIGHT; row++)
     {
          for(int col=0; col<SCREEN_WIDTH; col++)
          {
               display[col][row] = 0;
          }
     }
     //Initialize Graphics Buffer
     for(int index=0; index<SCREEN_SIZE; index++)
     {
          gfxBuffer[index] = 0;
     }
     //Initialize window
     window_init();
}

void window_init(void)
{
     windowWidth = SCREEN_WIDTH * windowScale;
     windowHeight = SCREEN_HEIGHT * windowScale;
     windowSize = windowWidth * windowHeight;
     window = NULL;

     //Initialize Window using SDL
     if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
     {
         printf("Error Starting SDL Video Subsystem: %s\n", SDL_GetError());
         exit(1);
     }
     window = SDL_CreateWindow("CHIP-8 Emulator",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
     if (window == NULL){
         printf("Error Creating SDL window: %s\n", SDL_GetError());
         exit(1);
     }
     //Disable Vsync
     SDL_GL_SetSwapInterval(0);
     renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
     SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);

     texture = SDL_CreateTexture(renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            SCREEN_WIDTH, SCREEN_HEIGHT);
}

void draw_screen(void)                  //Draw display to screen, set drawFlag to off.
{
//TODO ENABLE COLOUR CHANGE FOR PIXELS
     for(int indexH=0; indexH<SCREEN_HEIGHT; indexH++)
     {
          for(int indexW=0; indexW<SCREEN_WIDTH; indexW++)
          {
               uint32_t pixel = display[indexW][indexH];
               gfxBuffer[(SCREEN_WIDTH*indexH)+indexW] = (pixel*(SPRITE_COLOUR^BG_COLOUR))^BG_COLOUR;
               /*if(pixel)
               {
                    gfxBuffer[(SCREEN_WIDTH*indexH)+indexW] = SPRITE_COLOUR;
               }
               else
               {
                    gfxBuffer[(SCREEN_WIDTH*indexH)+indexW] = BG_COLOUR;
               }
               gfxBuffer[(SCREEN_WIDTH*indexH)+indexW] = (pixel * WHITE) & WHITE;
               */
          }
     }

     SDL_UpdateTexture(texture, NULL, gfxBuffer, SCREEN_WIDTH*sizeof(Uint32));
     SDL_RenderClear(renderer);
     SDL_RenderCopy(renderer, texture, NULL, NULL);
     SDL_RenderPresent(renderer);
     drawFlag = false;
}

void setBackgroundColour(char* colour)  //Sets the background colour
{
     if (strcmp(colour, "red") == 0)
     {
          BG_COLOUR = RED;
     }
     else if (strcmp(colour, "blue") == 0)
     {
          BG_COLOUR = BLUE;
     }
     else if (strcmp(colour, "green") == 0)
     {
          BG_COLOUR = GREEN;
     }
     else if (strcmp(colour, "purple") == 0)
     {
          BG_COLOUR = RED|BLUE;
     }
     else if (strcmp(colour, "aqua") == 0)
     {
          BG_COLOUR = GREEN|BLUE;
     }
     else if (strcmp(colour, "yellow") == 0)
     {
          BG_COLOUR = GREEN|RED;
     }
     else //Default black
     {
          BG_COLOUR = BLACK;
     }
}
void setSpriteColour(char* colour)      //Sets the sprite colour
{
     if (strcmp(colour, "red") == 0)
     {
          SPRITE_COLOUR = RED;
     }
     else if (strcmp(colour, "blue") == 0)
     {
          SPRITE_COLOUR = BLUE;
     }
     else if (strcmp(colour, "green") == 0)
     {
          SPRITE_COLOUR = GREEN;
     }
     else if (strcmp(colour, "purple") == 0)
     {
          SPRITE_COLOUR = RED|BLUE;
     }
     else if (strcmp(colour, "aqua") == 0)
     {
          SPRITE_COLOUR = GREEN|BLUE;
     }
     else if (strcmp(colour, "yellow") == 0)
     {
          SPRITE_COLOUR = GREEN|RED;
     }
     else //Default white
     {
          SPRITE_COLOUR = WHITE;
     }
}
