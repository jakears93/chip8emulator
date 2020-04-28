/*
     File: cpu.h
     Author: Jacob Arsenault
     Date: Apr 21,2020
*/
#ifndef CPU_H
#define CPU_H

//Dependencies
#include <stdint.h>

//Constants
#define RUN 1
#define STD_FRAME_TIME (17*1000)
#define SECS_TO_USECS (1000*1000)
#define MAX_RAM_SIZE 4096
#define STACK_SIZE 16
#define NUM_OF_REGISTERS 16

//Structures
struct registers_s
{
     uint8_t V[NUM_OF_REGISTERS];  //16 All purpose registers
     uint16_t I;                    //Used to store memory addresses (only 12 bits used)
     uint8_t DT;                    //Delay Timer register
     uint8_t ST;                   //Sound Timer register
} typedef reg;

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

//Functions Declarations
void cpu_init(void);                         //Initialize cpu/pc/sp
uint16_t fetchInstruction(void);             //Retrieve instruction
void runCycle(uint16_t instruction);         //Decode and execute instruction
void loadRom(char* filePath);                //Load rom into memory

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
void ADD_VxKk(uint8_t x, uint8_t kk);            //Add kk to Vx
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
void DRW(uint8_t x, uint8_t y, uint8_t n);    //Display n-byte sprite at memory location I at [Vx,Vy]
void SKP_Vx(uint8_t x);                       //Skip if Key with value of Vx is pressed
void SKNP_Vx(uint8_t x);                     //Skip if Key with value of Vx is not pressed
void LD_VxDT(uint8_t x);                     //Load DT register value into Vx
void LD_VxKey(uint8_t x);                    //Wait for key press, store valye in Vx
void LD_DTVx(uint8_t x);                     //Load Vx into DT Register
void LD_STVx(uint8_t x);                     //Load Vx into ST Register
void ADD_IVx(uint8_t x);                     //Add Vx to I
void LD_IVx(uint8_t x);                      //Set I = location of sprite for digit Vx
void LD_BCD(uint8_t x);                       //Store BCD representation of Vx in I, I+1, I+2
void LD_I(uint8_t x);                        //Store V[0] through V[x] into memory starting at I
void LD_VxI(uint8_t x);                        //Store values from memory starting at I into V[0]-V[x]
//Super Chip-48 instructions to be added below

#endif
