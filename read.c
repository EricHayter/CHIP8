#include <stdint.h>
#include <stdio.h>
#include "read.h"

uint16_t mergeinstruction(uint8_t b1, uint8_t b0)
{
    uint16_t instruction = b1;
    instruction *= 0x100;
    return instruction + b0;
}

int readfile(const char *filename, uint8_t *instructions)
{
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("error: could not read file %s", filename);
        return 1;
    }

    fseek(fp, 0L, SEEK_END);
    long filesize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    for (long i = 0L; i < filesize; i++) {
        instructions[i+512] = fgetc(fp);
    }

    fclose(fp);
    return 0;
}
