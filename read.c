#include <stdint.h>
#include <stdio.h>
#include "read.h"

uint16_t mergeinstruction(uint8_t b1, uint8_t b0)
{
    uint16_t instruction = b1;
    instruction *= 0x100;
    return instruction + b0;
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
