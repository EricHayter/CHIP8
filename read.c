#include <stdio.h>
#include <inttypes.h>

#define ADDRESS_OFFSET 0x200
#define INSTRUCTION_SIZE 0xFFF - ADDRESS_OFFSET

int readfile(const char *filename, uint16_t *instructions);
uint16_t mergeinstruction(uint8_t b1, uint8_t b0);
void parseinstruction(uint8_t *instruction);
uint8_t getfirstbyte(uint16_t instruction);

int main(int argc, char **argv)
{
    printf("%04X\n", getfirstbyte(0x00EE));
    printf("%04X\n", getfirstbyte(0x20EE));

    uint16_t *instructionpointer;
    char *filename = argv[1];
    uint16_t instructions[INSTRUCTION_SIZE]; // 0-255 values
    if (readfile(filename, instructions)) // reading failed
        return 1;
    for (int i = 0; i < 20; i++) {
         printf("%04X\n", instructions[i]);
    }

    instructionpointer = &instructions[0]; 
}

uint16_t mergeinstruction(uint8_t b1, uint8_t b0)
{
    uint16_t instruction = b1;
    instruction *= 0x100;
    return instruction + b0;
}

void parseinstruction(uint8_t *instruction)
{
    uint8_t firstb = getfirstbyte(*instruction);
    switch(firstb) {
        case 0x00:
            // 0nnn
            if (*instruction == 0x00E0) {
                // clear the display
            } else if (*instruction == 0x00EE) {
                // return to the top of the stack
            }
            break;
    }
}

uint8_t getfirstbyte(uint16_t instruction)
{
    return instruction / 0x1000;
}

int readfile(const char *filename, uint16_t *instructions)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("error: could not read file %s", filename);
        return 1;
    }

    fseek(fp, 0L, SEEK_END);
    long filesize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    uint8_t c;
    for (long i = 0L; i < filesize/2; i++) {
        c = fgetc(fp);
        instructions[i] = mergeinstruction(c, fgetc(fp));
    }

    fclose(fp);
    return 0;
}
