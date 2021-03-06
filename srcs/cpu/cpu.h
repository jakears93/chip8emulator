/*
     File: cpu.h
     Author: Jacob Arsenault
     Date: Apr 21,2020
*/
#ifndef CPU_H
#define CPU_H

//Dependencies
#include <stdint.h>
#include <stdbool.h>

//Constants
#define MIN_FRAME_RATE 30
#define STD_FRAME_RATE 1000
#define SECS_TO_USECS (1000000)
#define MAX_RAM_SIZE 4096
#define STACK_SIZE 16
#define NUM_OF_REGISTERS 16
#define WIDTH 64
#define HEIGHT 32
#define FONT_OFFSET 5
#define FONT_SIZE 80
#define EVENT_DELAY 8
#define DRAW_DELAY 8
#define TIMER_DELAY 16

//Structures
struct registers_s
{
     uint8_t V[NUM_OF_REGISTERS];  //16 All purpose registers
     uint16_t I;                    //Used to store memory addresses (only 12 bits used)
     uint8_t DT;                    //Delay Timer register
     uint8_t ST;                   //Sound Timer register
} typedef reg;

//Emulation Control Flags
struct controlFlags_s{
     bool RESET;
     bool RUN;
     bool PAUSE;
     bool PAUSE_SET;
} typedef cFlags;

//Config Flags
struct configFlags_s{
     int videoSizeFlag;
     float frameRateFlag;
     char* bgColourFlag;
     char* spriteColourFlag;
} typedef configFlags;

//Set up control and config flags
static cFlags controlFlags = {0,1,0,0};
static configFlags flags = {0,-1, "black", "white"};

//Global Variables
char* romPath;
int frameTime;
uint8_t RAM[MAX_RAM_SIZE];      //4kB of memory
/* RAM layout:
0x000 - 0x1FF reserved for interpreter
0x200 - 0xFFF reserved for programs
     0x200 start of most chip8 programs
     0x600 start of eti 660 chip8 programs
*/
uint16_t PC;                   //Program Counter
uint16_t SP;                   //Stack Pointer (Ponnts to top of the stack)
uint16_t STACK[STACK_SIZE];    //Stack
reg REG;                      //Register Structure
bool drawFlag;                //Draws to page if set
uint8_t display[WIDTH][HEIGHT];

static uint8_t fontSprites[FONT_SIZE] =      //Font Sprites to be stored in RAM
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

//Functions Declarations
void cpu_init(void);                         //Initialize cpu/pc/sp
uint16_t fetchInstruction(void);             //Retrieve instruction
void runCycle(uint16_t instruction);         //Decode and execute instruction
void loadRom(char* filePath);                //Load rom into memory
void rom_reset(void);                        //Resets CPU to beginning of ROM
void updateDT(void);                         //Updates REG.DT value
uint8_t updateST(void);                      //Updates REG.ST value, returns value

//Opcode Functions for Chip-8 as per http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
void SYS(uint16_t nnn);                      //Jump to machine code routine at nnn
void CLS(void);                              //Clears display
void RET(void);                              //Return from subroutine
void JP(uint16_t nnn);                       //Jump to location nnn
void CALL(uint16_t nnn);                     //Call subroutine at nnn
void SE_VxKk(uint8_t x, uint8_t kk);         //Skip if Vx = kk
void SNE_VxKk(uint8_t x, uint8_t kk);        //Skip if Vx != kk
void SE_VxVy(uint8_t x, uint8_t y);          //Skip if Vx = Vy
void LD_VxKk(uint8_t x, uint8_t kk);         //Load kk to Vx
void ADD_VxKk(uint8_t x, uint8_t kk);        //Add kk to Vx
void LD_VxVy(uint8_t x, uint8_t y);          //Store Vy in Vx
void OR_VxVy(uint8_t x, uint8_t y);          //Bitwise OR Vx,Vy. Store in Vx
void AND_VxVy(uint8_t x, uint8_t y);         //Bitwise AND Vx,Vy. Store in Vx
void XOR_VxVy(uint8_t x, uint8_t y);         //Bitwise XOR Vx,Vy. Store in Vx
void ADD_VxVy(uint8_t x, uint8_t y);         //Add Vy to Vx
void SUB_VxVy(uint8_t x, uint8_t y);         //Subtract Vy from Vx
void SHR_Vx(uint8_t x);                      //Shift Vx Right 1 Bit
void SUBN_VxVy(uint8_t x, uint8_t y);        //Subtract Vx from Vy, store in Vx
void SHL_Vx(uint8_t x);                      //Shift Vx Left 1 Bit
void SNE_VxVy(uint8_t x, uint8_t y);         //Skip if Vx != Vy
void LD_Innn(uint16_t nnn);                  //Load nnn to I
void JP_V0(uint16_t nnn);                    //Jump to nnn+V[0]
void RND_VxKk(uint8_t x, uint8_t kk);        //Random number AND'ed with kk, stored in Vx
void DRW(uint8_t x, uint8_t y, uint8_t n);   //Display n-byte sprite at memory location I at [Vx,Vy]
void SKP_Vx(uint8_t x);                      //Skip if Key with value of Vx is pressed
void SKNP_Vx(uint8_t x);                     //Skip if Key with value of Vx is not pressed
void LD_VxDT(uint8_t x);                     //Load DT register value into Vx
void LD_VxKey(uint8_t x);                    //Wait for key press, store valye in Vx
void LD_DTVx(uint8_t x);                     //Load Vx into DT Register
void LD_STVx(uint8_t x);                     //Load Vx into ST Register
void ADD_IVx(uint8_t x);                     //Add Vx to I
void LD_IVx(uint8_t x);                      //Set I = location of sprite for digit Vx
void LD_BCD(uint8_t x);                      //Store BCD representation of Vx in I, I+1, I+2
void LD_I(uint8_t x);                        //Store V[0] through V[x] into memory starting at I
void LD_VxI(uint8_t x);                      //Store values from memory starting at I into V[0]-V[x]
//Super Chip-48 instructions to be added below

#endif
