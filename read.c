#include <stdio.h>
#include <inttypes.h>

#define ADDRESS_OFFSET 0x200
#define INSTRUCTION_SIZE 0xFFF - ADDRESS_OFFSET

uint8_t instructions[INSTRUCTION_SIZE]; // used to store the game instructions

FILE *fp;
int filesize;

int main()
{
    fp = fopen("Tetris.ch8", "r"); 

    fseek(fp, 0L, SEEK_END); // find the size of the file
    filesize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    int i = 0;
    uint8_t c = fgetc(fp);
    
    for (int i = 0; i < filesize; ++i) {
        instructions[i] = fgetc(fp);
    }


    return 0;
}
