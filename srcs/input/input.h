/*
     File: input.h
     Author: Jacob Arsenault
     Date: Apr 26,2020
*/
#ifndef INPUT_H
#define INPUT_H

//Dependencies
#include <stdbool.h>

//Constants
/*   Original Keypad Layout with (index)
     1(0)    2(1)    3(2)    C(3)
     4(4)    5(5)    6(6)    D(7)
     7(8)    8(9)    9(10)   E(11)
     A(12)   0(13)   b(14)   F(15)
*/
#define INPUT_CONFIG_FILE "inputConfig.txt"
#define NUM_OF_KEYS 16
#define PRESSED 1
#define RELEASED 0
#define KEY_1 0x1
#define KEY_2 0x2
#define KEY_3 0x3
#define KEY_C 0xC
#define KEY_4 0x4
#define KEY_5 0x5
#define KEY_6 0x6
#define KEY_D 0xD
#define KEY_7 0x7
#define KEY_8 0x8
#define KEY_9 0x9
#define KEY_E 0xE
#define KEY_A 0xA
#define KEY_0 0x0
#define KEY_B 0xB
#define KEY_F 0xF

//Global Variables
int KEYS[NUM_OF_KEYS];
bool keyState[NUM_OF_KEYS];
char keyMap[NUM_OF_KEYS];

//Function Declarations
int input_init(void);         //Initialize the inputs
void debug_printKeys(void);   //Print Key Values and States
int getKeyPress(void);        //Wait and read a key press, return the key that was press
void toggle_key(char key);    //Toggle key state

#endif
