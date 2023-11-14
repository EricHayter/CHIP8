#ifndef READ_H
#define READ_H

#include <stdint.h>
uint16_t mergeinstruction(uint8_t b1, uint8_t b0);
int readfile(const char *filename, uint8_t *instructions);

#endif
