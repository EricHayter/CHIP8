#include "screen.h"
#include <stdint.h>
#include <stdio.h>
#include <curses.h>

static uint64_t screen[32]; 
                     
void printscreen()
{
    clear();
    for (int i = 0; i < 32; i++) {
        for (int j = 63; j >= 0; j--) {
            if ((screen[i] >> j) & 1)
                printw("#");
            else
                printw(" ");
        }
        printw("\n");
    }
    refresh();
}

uint8_t printsprite(uint8_t *sprite, uint8_t size, uint8_t x, uint8_t y) {
    uint8_t change;
    for (uint8_t i = 0; i < size; i++) {
        change = loadbytetoscreen(*(sprite+i), x, y+i);
    }
    printscreen();
    return change;
}

uint8_t loadbytetoscreen(uint8_t b, uint8_t x, uint8_t y) {
    uint64_t nb = 0x0;
    for (uint8_t i = 0; i < sizeof(uint8_t); i++) {
       nb += (uint64_t)(b) << ((127 - x - i) % 64); 
    }
    uint64_t prev = screen[y];
    screen[y] = nb ^ screen[y];   
    return (~screen[y] & prev) != 0;
}

void clearscreen()
{
    for (int i = 0; i < 32; i++)
        screen[i] = 0;
}
