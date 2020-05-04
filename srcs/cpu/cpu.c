/*
     File: cpu.c
     Author: Jacob Arsenault
     Date: Apr 21,2020
*/

//Dependencies
#include <stdlib.h>
#include <stdio.h>
#include "cpu.h"
#include "../input/input.h"

//Functions Definitions
void cpu_init(void)                           //Initialize CPU
{
     //Initialize RAM contents
     for(int i=0; i<MAX_RAM_SIZE; i++)
     {
          RAM[i] = 0;
     }
     //Load font sprites into memory at location 0x000 - 0x079
     for(int index=0; index<FONT_SIZE; index++)
     {
          RAM[index] = fontSprites[index];
     }
     //Set program counter and stack pointer
     PC = 0x200;
     SP = 0;
     //Initialize Stack and registers
     for(int i=0; i<STACK_SIZE; i++)
     {
          STACK[i] = 0;
     }
     for(int i=0; i<NUM_OF_REGISTERS; i++)
     {
          REG.V[i] = 0;
     }
     REG.I=0;
     REG.DT=0x00;
     REG.ST=0x00;
     drawFlag = false;
     //Load the rom
     loadRom(romPath);
}

void rom_reset(void)                         //Reset CPU to beginning of ROM
{
     CLS();
     cpu_init();
}

uint16_t fetchInstruction(void)              //Read instruction, increment PC to get ready for next fetch.
{
     uint16_t instruction = (RAM[PC] << 8) | RAM[PC+1];
     PC+=2;
     return instruction;
}

void loadRom(char* filePath)                 //Load rom file into memory
{
     FILE* rom = fopen(filePath, "rb");
     if(rom == NULL)
     {
          printf("Failed to open ROM: %s. Exiting program.\n",filePath);
          exit(1);
     }
     fseek(rom, 0, SEEK_END);
     int romSize = ftell(rom);
     rewind(rom);

     uint8_t* romBuffer = (uint8_t*)malloc(sizeof(uint8_t)*romSize);
     if(romBuffer == NULL)
     {
          printf("Failed to allocate memory for the rom: %s.  Exiting program.\n",filePath);
          exit(1);
     }
     size_t status = fread(romBuffer, sizeof(uint8_t), (size_t)romSize, rom);
     if(status != romSize)
     {
          printf("Failed to copy rom: %s.  Exiting program.\n",filePath);
          exit(1);
     }
     if(romSize < (MAX_RAM_SIZE-0x200))
     {
          for(int i=0; i<romSize; i++)
          {
               RAM[i+0x200] = romBuffer[i];
          }
     }
     else
     {
          printf("Rom: %s is too large for emulator. Exiting program.\n",filePath);
          exit(1);
     }
     fclose(rom);
     free(romBuffer);
}

void updateDT(void)                          //Updates REG.DT value
{
     if(REG.DT != 0)
          REG.DT = REG.DT - 1;

}
uint8_t updateST(void)                       //Updates REG.ST value
{
     if(REG.ST != 0)
          REG.ST = REG.ST-1;
     return REG.ST;
}

void runCycle(uint16_t instruction)          //Decode opcode and execute function call.
{
     uint8_t n = instruction & 0x000F;
     uint8_t x = (instruction & 0x0F00) >> 8;
     uint8_t y = (instruction & 0x00F0) >> 4;
     uint8_t kk = instruction & 0x00FF;
     uint16_t nnn = instruction & 0x0FFF;

     #ifdef DEBUG
          printf("opcode: 0x%X\n",instruction);
          printf("\tx= %X, y=%X, n=%X, kk=%X, nnn=%X\n", x, y, n, kk, nnn);
          if((instruction & 0xF000) == 0xD000)
               printf("\tDrawing To Screen\n");
     #endif

     switch (instruction & 0xF000)
     {
          case 0x0000:
               switch (instruction & 0x0FFF)
               {
                    case 0x00E0:   CLS();    break;
                    case 0x00EE:   RET();    break;
                    default:       printf("Unknown Instruction: %d\n",instruction);
               }
               break;
          case 0x1000:   JP(nnn);            break;
          case 0x2000:   CALL(nnn);          break;
          case 0x3000:   SE_VxKk(x, kk);     break;
          case 0x4000:   SNE_VxKk(x, kk);    break;
          case 0x5000:   SE_VxVy(x, y);      break;
          case 0x6000:   LD_VxKk(x, kk);     break;
          case 0x7000:   ADD_VxKk(x, kk);    break;
          case 0x8000:
               switch (instruction & 0x000F)
               {
                    case 0x0000:   LD_VxVy(x, y);      break;
                    case 0x0001:   OR_VxVy(x, y);      break;
                    case 0x0002:   AND_VxVy(x, y);     break;
                    case 0x0003:   XOR_VxVy(x, y);     break;
                    case 0x0004:   ADD_VxVy(x, y);     break;
                    case 0x0005:   SUB_VxVy(x, y);     break;
                    case 0x0006:   SHR_Vx(x);          break;
                    case 0x0007:   SUBN_VxVy(x, y);    break;
                    case 0x000E:   SHL_Vx(x);          break;
                    default:       printf("Unknown Instruction: %d\n",instruction);
               }
               break;
          case 0x9000:   SNE_VxVy(x, y);     break;
          case 0xA000:   LD_Innn(nnn);       break;
          case 0xB000:   JP_V0(nnn);         break;
          case 0xC000:   RND_VxKk(x, kk);    break;
          case 0xD000:   DRW(x, y, n);       break;
          case 0xE000:
               switch (instruction & 0x00FF)
               {
                    case 0x009E:   SKP_Vx(x);     break;
                    case 0x00A1:   SKNP_Vx(x);    break;
                    default:       printf("Unknown Instruction: %d\n",instruction);
               }
               break;
          case 0xF000:
               switch (instruction & 0x00FF)
               {
                    case 0x0007:   LD_VxDT(x);    break;
                    case 0x000A:   LD_VxKey(x);   break;
                    case 0x0015:   LD_DTVx(x);    break;
                    case 0x0018:   LD_STVx(x);    break;
                    case 0x001E:   ADD_IVx(x);    break;
                    case 0x0029:   LD_IVx(x);     break;
                    case 0x0033:   LD_BCD(x);     break;
                    case 0x0055:   LD_I(x);       break;
                    case 0x0065:   LD_VxI(x);     break;
                    default:       printf("Unknown Instruction: %d\n",instruction);
               }
               break;
          default:  printf("Error in opcode handling. Unknown Instruction: %d\n",instruction);
     }
}

//Opcode Functions for Chip-8 as per http://devernay.free.fr/hacks/chip8/C8TECH10.HTM
void SYS(uint16_t nnn)                       //Jump to machine code routine at nnn
{
     //Unused instruction as per documentation but implemented anyways
     PC = nnn;
}
void CLS(void)                              //Clears display
{
     for(int row=0; row<WIDTH; row++)
     {
          for(int col=0; col<HEIGHT; col++)
          {
               display[row][col] = 0;
          }
     }
     drawFlag = true;
}
void RET(void)                              //Return from subroutine
{
     PC = STACK[SP];
     SP--;
}
void JP(uint16_t nnn)                       //Jump to location nnn
{
     PC = nnn;
}
void CALL(uint16_t nnn)                     //Call subroutine at nnn
{
     ++SP;
     STACK[SP] = PC;
     PC = nnn;
}
void SE_VxKk(uint8_t x, uint8_t kk)         //Skip if Vx = kk
{
     if(REG.V[x] == kk)
          PC+=2;
}
void SNE_VxKk(uint8_t x, uint8_t kk)        //Skip if Vx != kk
{
     if(REG.V[x] != kk)
          PC+=2;
}
void SE_VxVy(uint8_t x, uint8_t y)          //Skip if Vx = Vy
{
     if(REG.V[x] == REG.V[y])
          PC+=2;
}
void LD_VxKk(uint8_t x, uint8_t kk)         //Load kk to Vx
{
     REG.V[x] = kk;
}
void ADD_VxKk(uint8_t x, uint8_t kk)        //Add kk to Vx
{
     REG.V[x] = REG.V[x] + kk;
}
void LD_VxVy(uint8_t x, uint8_t y)          //Store Vy in Vx
{
     REG.V[x] = REG.V[y];
}
void OR_VxVy(uint8_t x, uint8_t y)          //Bitwise OR Vx,Vy. Store in Vx
{
     REG.V[x] = (REG.V[x] | REG.V[y]);
}
void AND_VxVy(uint8_t x, uint8_t y)         //Bitwise AND Vx,Vy. Store in Vx
{
     REG.V[x] = (REG.V[x] & REG.V[y]);
}
void XOR_VxVy(uint8_t x, uint8_t y)         //Bitwise XOR Vx,Vy. Store in Vx
{
     REG.V[x] = (REG.V[x] ^ REG.V[y]);
}
void ADD_VxVy(uint8_t x, uint8_t y)         //Add Vy to Vx
{
     uint16_t sum = REG.V[x] + REG.V[y];
     if(sum > 255)
          REG.V[0xF]=1;
     else
          REG.V[0xF]=0;
     REG.V[x] = REG.V[x] + REG.V[y];
}
void SUB_VxVy(uint8_t x, uint8_t y)         //Subtract Vy from Vx
{
     if(REG.V[x] > REG.V[y])
          REG.V[0xF] = 1;
     else
          REG.V[0xF] = 0;
     REG.V[x] = REG.V[x] - REG.V[y];
}
void SHR_Vx(uint8_t x)                       //Shift Vx Right 1 Bit
{
     if((REG.V[x] & 0x01) == 1)
          REG.V[0xF] = 1;
     else
          REG.V[0xF] = 0;
     REG.V[x] = REG.V[x] >> 1;
}
void SUBN_VxVy(uint8_t x, uint8_t y)         //Subtract Vx from Vy, store in Vx
{
     if(REG.V[y] > REG.V[x])
          REG.V[0xF] = 1;
     else
          REG.V[0xF] = 0;
     REG.V[x] = REG.V[y] - REG.V[x];
}
void SHL_Vx(uint8_t x)                       //Shift Vx Left 1 Bit
{
     if(((REG.V[x] & 0x80) >> 7) == 1)
          REG.V[0xF] = 1;
     else
          REG.V[0xF] = 0;
     REG.V[x] = REG.V[x] << 1;
}
void SNE_VxVy(uint8_t x, uint8_t y)         //Skip if Vx != Vy
{
     if (REG.V[x] != REG.V[y])
          PC+=2;
}
void LD_Innn(uint16_t nnn)                  //Load nnn to I
{
     REG.I = nnn;
}
void JP_V0(uint16_t nnn)                    //Jump to nnn+V[0]
{
     PC = nnn + REG.V[0];
}
void RND_VxKk(uint8_t x, uint8_t kk)              //Random number AND'ed with kk, stored in Vx
{
     REG.V[x] = (rand()%256) & kk;
}
void DRW(uint8_t x, uint8_t y, uint8_t n)         //Display n-byte sprite at memory location I at [Vx,Vy]
{
     REG.V[0xF] = 0;
     uint8_t spriteByte = 0x00;
     uint8_t pixelOn;
     for(uint8_t yLine=0; yLine<n; yLine++)           //Loop through sprite bytes
     {
          pixelOn = 0;
          spriteByte = RAM[(REG.I + yLine)];      //Load byte from RAM

          for(uint8_t xPix=0; xPix<8; xPix++)         //Loop through sprite bits
          {
               uint8_t xLoc = REG.V[x] + xPix;
               uint8_t yLoc = REG.V[y] + yLine;

               if(xLoc >= WIDTH)           //Check if X coordinate wraps around screen
               {
                    xLoc = xLoc - WIDTH;
               }
               if(yLoc >= HEIGHT)          //Check if Y coordinate wraps around screen
               {
                    yLoc = yLoc - HEIGHT;
               }
                                                       //Shift sprite byte so bit can be read
               pixelOn = (spriteByte >> (7-xPix)) & 0x01;  //Set pixelOn to on if sprite bit is set
               if(display[xLoc][yLoc] & pixelOn)      //If the sprite bit and existing screen pixel are both set, turn on collison
               {
                    REG.V[0xF] = 1;
               }
               display[xLoc][yLoc] ^= pixelOn;    //Xor pixel to be displayed
          }
     }
     drawFlag = true;
}
void SKP_Vx(uint8_t x)	                     //Skip if Key with value of Vx is pressed
{
     bool checkKey = true;
     int index = 0;
     while(checkKey)
     {
          if(REG.V[x] == KEYS[index])
          {
               if(keyState[index] == 1)
                    PC = PC + 2;
               checkKey = false;
          }
          index++;
     }
}
void SKNP_Vx(uint8_t x)                      //Skip if Key with value of Vx is not pressed
{
     bool checkKey = true;
     int index = 0;
     while(checkKey)
     {
          if(REG.V[x] == KEYS[index])
          {
               if(keyState[index] == 0)
                    PC = PC + 2;
               checkKey = false;
          }
          index++;
     }
}
void LD_VxDT(uint8_t x)                     //Load DT register value into Vx
{
     REG.V[x] = REG.DT;
}
void LD_VxKey(uint8_t x)                    //Wait for key press, store value in Vx
{
     int keyVal = getKeyPress();
     if(keyVal == -1)
     {
          PC-=2;
	  return;
     }
     REG.V[x] = keyVal;
}
void LD_DTVx(uint8_t x)                     //Load Vx into DT Register
{
     REG.DT = REG.V[x];
}
void LD_STVx(uint8_t x)                     //Load Vx into ST Register
{
     REG.ST = REG.V[x];
}
void ADD_IVx(uint8_t x)                     //Add Vx to I
{
     REG.I = REG.I + REG.V[x];
}
void LD_IVx(uint8_t x)                      //Set I = location of sprite for digit Vx
{
     REG.I = REG.V[x]*FONT_OFFSET;
}
void LD_BCD(uint8_t x)                      //Store BCD representation of Vx in I, I+1, I+2
{
     RAM[REG.I] = (REG.V[x] / 100) % 10;
     RAM[REG.I+1] = (REG.V[x] / 10) % 10;
     RAM[REG.I+2] = (REG.V[x] % 10);
}
void LD_I(uint8_t x)                        //Store V[0] through V[x] into memory starting at I
{
     for(int count=0; count<=x; count++)
     {
          RAM[REG.I+count] = REG.V[count];
     }
}
void LD_VxI(uint8_t x)                      //Store values from memory starting at I into V[0]-V[x]
{
     for(int count=0; count<=x; count++)
     {
          REG.V[count] = RAM[REG.I+count];
     }
}
