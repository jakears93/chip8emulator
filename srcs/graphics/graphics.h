/*
     File: graphics.h
     Author: Jacob Arsenault
     Date: Apr 26,2020
*/
#ifndef GRAPHICS_H
#define GRAPHICS_H

//Dependencies
#include <stdbool.h>
#include <stdint.h>
#include "SDL2/SDL.h"

//Constants
#define DEFAULT_SCALE 15
#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define SCREEN_SIZE 2048
#define WHITE 0xFFFFFFFF
#define BLACK 0x00000000
#define BLUE 0xFF0000FF
#define GREEN 0x0000FF00
#define RED 0xFFFF0000

//Global Variables
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Texture* texture;
uint32_t gfxBuffer[SCREEN_SIZE];

uint32_t BG_COLOUR;
uint32_t SPRITE_COLOUR;

int windowWidth;
int windowHeight;
int windowSize;
int windowScale;

//Function Declarations
void graphics_init(void);               //Set pixels to 0, load font sprites to ram
void window_init(void);                 //Initialize SDL window
void draw_screen(void);                 //Draw the updated screen
void setBackgroundColour(char* colour); //Sets the background colour
void setSpriteColour(char* colour);     //Sets the sprite colour

#endif
