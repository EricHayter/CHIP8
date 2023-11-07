#include <stdio.h>
#include <inttypes.h>

#define ADDRESS_OFFSET 0x200
#define INSTRUCTION_SIZE 0xFFF - ADDRESS_OFFSET

int readinstructions(const char *filename, uint8_t *instructions);
uint16_t createinstruction(uint8_t b1, uint8_t b0);

int main(int argc, char **argv)
{
    char *filename = argv[1];
    uint8_t instructions[INSTRUCTION_SIZE]; // 0-255 values
    if (readinstructions(filename, instructions)) // reading failed
        return 1;
    for (int i = 0; i < 20; i++) {
        printf("%02X\n", instructions[i]);
    }
}

uint16_t createinstruction(uint8_t b1, uint8_t b0)
{
    uint16_t instruction = b1;
    instruction <<= 8;
    return instruction + b0;
}


int readinstructions(const char *filename, uint8_t *instructions)
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
    for (long i = 0L; i < filesize; i++) {
        instructions[i] = fgetc(fp);
    }

    fclose(fp);
    return 0;
}
