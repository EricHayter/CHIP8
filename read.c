#include <stdio.h>
#include <inttypes.h>

#define ADDRESS_OFFSET 0x200
#define INSTRUCTION_SIZE 0xFFF - ADDRESS_OFFSET

int readfile(const char *filename, uint16_t *instructions);
uint16_t mergeinstruction(uint8_t b1, uint8_t b0);
void parseinstruction(uint16_t *instruction);
void printscreen();
void clearscreen();

uint8_t gr[16]; // general purpose register
uint16_t ir;    // "I" register
uint8_t sr, dr; // sound and delay registers
uint64_t screen[32]; // 64x32 screensize monochrome
uint16_t pc = 0x0;   // program counter (TODO check if this needs an offset)

int main(int argc, char **argv)
{
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

void parseinstruction(uint16_t *instruction)
{
    switch(*instruction / 0x1000) {
        case 0x0:
            // 0nnn
            if (*instruction == 0x00E0) {
                clearscreen();
            } else if (*instruction == 0x00EE) {
                // return to the top of the stack
            }
            break;
        case 0x1:
           pc = *instruction % 1000;
           break;

    }
}

void printscreen()
{
    for (int i = 0; i < 32; i++) {
        uint64_t cp = screen[i]; 
        while (cp) {
            if (cp >> 63 && 1)
                printf("#");
            else
                printf(" ");
        }
        printf("\n");
    }
}

void clearscreen()
{
    for (int i = 0; i < 32; i++)
        screen[i] = 0;
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
