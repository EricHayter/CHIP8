#ifndef SCREEN_H
#define SCREEN_H

#include <stdint.h>
void printscreen();
void clearscreen();
uint8_t printsprite(uint8_t *sprite, uint8_t size, uint8_t x, uint8_t y);
uint8_t loadbytetoscreen(uint8_t b, uint8_t x, uint8_t y);

#endif
