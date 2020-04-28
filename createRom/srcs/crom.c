/*
   File: crom.c
   Author: Jake Arsenault
   Date: Tuesday Apr 28, 2020   14:11 PM
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char * argv[],char * envp[])
{
	char filePath[150];
	printf("Enter a file path: \n");
	scanf("%s",filePath);
	printf("File Path: %s\n",filePath);
	FILE *fout;
	uint16_t op;
	uint8_t highByte, lowByte;
	char buf[4];
	fout = fopen(filePath,"wb+");
	if(fout == NULL)
	{
		printf("Failed to open file for writing\n");
		exit(1);	
	}
	while (1)
	{
		for(int i=0; i<4; i++)
		{
			buf[i] = '\0';
		}
		printf("Opcode: ");
		scanf(" %s",buf);
		if(!(strcmp(buf, "ffff")))
			break;
		op = strtol(buf, NULL, 16); 
		printf("Opcode %x written.\n",op);
		highByte = (op&0xFF00) >> 8;
		lowByte = (op&0x00FF);
		fwrite(&highByte, 1, 1, fout);
		fwrite(&lowByte, 1, 1, fout);
	}
	fclose(fout);
	return 0;
}
