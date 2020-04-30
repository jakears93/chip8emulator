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
#define FONT_SIZE 80
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

int windowWidth;
int windowHeight;
int windowSize;
int windowScale;
static uint8_t fontSprites[FONT_SIZE] =
{
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0 starts at index 0
    0x20, 0x60, 0x20, 0x20, 0x70, //1 starts at index 5
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2 starts at index 10
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3 starts at index 15
    0x90, 0x90, 0xF0, 0x10, 0x10, //4 starts at index 20
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5 starts at index 25
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6 starts at index 30
    0xF0, 0x10, 0x20, 0x40, 0x40, //7 starts at index 35
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8 starts at index 40
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9 starts at index 45
    0xF0, 0x90, 0xF0, 0x90, 0x90, //A starts at index 50
    0xE0, 0x90, 0xE0, 0x90, 0xE0, //B starts at index 55
    0xF0, 0x80, 0x80, 0x80, 0xF0, //C starts at index 60
    0xE0, 0x90, 0x90, 0x90, 0xE0, //D starts at index 65
    0xF0, 0x80, 0xF0, 0x80, 0xF0, //E starts at index 70
    0xF0, 0x80, 0xF0, 0x80, 0x80  //F starts at index 75
};

//Function Declarations
void graphics_init(void);          //Set pixels to 0, load font sprites to ram
void window_init(void);            //Initialize SDL window
void draw_screen(void);            //Draw the updated screen

#endif
