#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>
void printscreen();
void clearscreen();
void printsprite(uint8_t *sprite, uint8_t size, uint8_t x, uint8_t y);
void printbyte(uint8_t b, uint8_t x, uint8_t y);

#endif
