#include <stdint.h>
#include <stdio.h>
#include "read.h"

uint16_t mergeinstruction(uint8_t b1, uint8_t b0)
{
    return (uint16_t)(b1) * 0x100 + b0;
}

int readfile(const char *filename, uint8_t *instructions)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("error: could not read file %s\n", filename);
        return 1;
    }

    fseek(fp, 0L, SEEK_END);
    long filesize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    for (long i = 0L; i < filesize; i++) {
        instructions[i+512] = fgetc(fp);
    }

    fclose(fp);

	// make sure to include the ascii characters for printing to
	// the screen. 
    
    // 0
    instructions[0] = 0xF0;    
    instructions[1] = 0x90;    
    instructions[2] = 0x90;    
    instructions[3] = 0x90;    
    instructions[4] = 0xF0;    

    // 1
    instructions[5] = 0x20;    
    instructions[6] = 0x60;    
    instructions[7] = 0x20;    
    instructions[8] = 0x20;    
    instructions[9] = 0x70;    

    // 2
    instructions[10] = 0xF0;    
    instructions[11] = 0x10;
    instructions[12] = 0xF0;    
    instructions[13] = 0x80;    
    instructions[14] = 0xF0;    


    // 3
    instructions[15] = 0xF0;    
    instructions[16] = 0x10;    
    instructions[17] = 0xF0;    
    instructions[18] = 0x10;    
    instructions[19] = 0xF0;    

    // 4
    instructions[20] = 0x90;    
    instructions[21] = 0x90;   
    instructions[22] = 0xF0;    
    instructions[23] = 0x10;    
    instructions[24] = 0x10;    

    // 5
    instructions[25] = 0xF0;    
    instructions[26] = 0x80;    
    instructions[27] = 0xF0;    
    instructions[28] = 0x10;    
    instructions[29] = 0xF0;    

    // 6
    instructions[30] = 0xF0;    
    instructions[31] = 0x80;    
    instructions[32] = 0xF0;    
    instructions[33] = 0x90;    
    instructions[34] = 0xF0;    

    // 7
    instructions[35] = 0xF0;    
    instructions[36] = 0x10;    
    instructions[37] = 0x20;    
    instructions[38] = 0x40;    
    instructions[39] = 0x40;    

    // 8
    instructions[40] = 0xF0;    
    instructions[41] = 0x90;    
    instructions[42] = 0xF0;    
    instructions[43] = 0x90;    
    instructions[44] = 0xF0;    

    // 9
    instructions[45] = 0xF0;    
    instructions[46] = 0x90;    
    instructions[47] = 0xF0;    
    instructions[48] = 0x10;    
    instructions[49] = 0x10;    

    // A
    instructions[50] = 0xF0;    
    instructions[51] = 0x90;    
    instructions[52] = 0xF0;    
    instructions[53] = 0x90;    
    instructions[54] = 0x90;    

    // B
    instructions[55] = 0xE0;    
    instructions[56] = 0x90;    
    instructions[57] = 0xE0;    
    instructions[58] = 0x90;    
    instructions[59] = 0xE0;    

    // C
    instructions[60] = 0xF0;    
    instructions[61] = 0x80;    
    instructions[62] = 0x80;    
    instructions[63] = 0x80;    
    instructions[64] = 0xF0;    

    // D
    instructions[65] = 0xE0;    
    instructions[66] = 0x90;    
    instructions[67] = 0x90;    
    instructions[68] = 0x90;    
    instructions[69] = 0xE0;    

    // E
    instructions[70] = 0xF0;    
    instructions[71] = 0x80;    
    instructions[72] = 0xF0;    
    instructions[73] = 0x80;    
    instructions[74] = 0xF0;    

    // F
    instructions[75] = 0xF0;    
    instructions[76] = 0x80;    
    instructions[77] = 0xF0;    
    instructions[78] = 0x80;    
    instructions[79] = 0x80;    

   return 0;
}
