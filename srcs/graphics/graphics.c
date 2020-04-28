/*
     File: graphics.c
     Author: Jacob Arsenault
     Date: Apr 26,2020
*/

//Dependencies
#include "graphics.h"
#include "../cpu/cpu.h"
#include <stdio.h>
#include <stdlib.h>

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
     //Load font sprites into memory at location 0x000 - 0x079
     for(int index=0; index<FONT_SIZE; index++)
     {
          RAM[index] = fontSprites[index];
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
     if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
     {
         printf("Error Starting SDL: %s\n", SDL_GetError());
         exit(1);
     }
     window = SDL_CreateWindow("CHIP-8 Emulator",SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN);
     if (window == NULL){
         printf("Error Creating SDL window: %s\n", SDL_GetError());
         exit(1);
     }
     renderer = SDL_CreateRenderer(window, -1, 0);
     SDL_RenderSetLogicalSize(renderer, windowWidth, windowHeight);

     texture = SDL_CreateTexture(renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            SCREEN_WIDTH, SCREEN_HEIGHT);

     draw_screen();
}

void draw_screen(void)                  //Draw display to screen, set drawFlag to off.
{
//TODO ENABLE COLOUR CHANGE FOR PIXELS
     for(int indexH=0; indexH<SCREEN_HEIGHT; indexH++)
     {
          for(int indexW=0; indexW<SCREEN_WIDTH; indexW++)
          {
               uint32_t pixel = display[indexW][indexH];
               gfxBuffer[(SCREEN_WIDTH*indexH)+indexW] = (pixel * WHITE) & WHITE;
          }
     }

     SDL_UpdateTexture(texture, NULL, gfxBuffer, SCREEN_WIDTH*sizeof(Uint32));
     SDL_RenderClear(renderer);
     SDL_RenderCopy(renderer, texture, NULL, NULL);
     SDL_RenderPresent(renderer);
     drawFlag = false;
}
