#include "screen.h"
#include <stdint.h>
#include <stdio.h>
#include <curses.h>

static uint64_t screen[32]; 
                     
void printscreen()
{
    for (int i = 0; i < 32; i++) {
        uint64_t cp = screen[i]; 
        while (cp) {
            if (cp >> 63 && 1)
                printw("#");
            else
                printw(" ");
            cp <<= 1;
        }
        printw("\n");
    }
    refresh();
}

void printsprite(uint8_t *sprite, uint8_t size, uint8_t x, uint8_t y) {
    for (uint8_t i = 0; i < size; i++) {
        printbyte(*(sprite+i), x, y+i);
    }
}

void printbyte(uint8_t b, uint8_t x, uint8_t y) {
    uint64_t nb = (uint64_t)(x);
    if (x <= 56) {
        nb <<= sizeof(uint64_t) - sizeof(x) - x;
    } else {
        nb = nb;
    }

    screen[y] = nb ^ screen[y];   
    printscreen();
}

void clearscreen()
{
    for (int i = 0; i < 32; i++)
        screen[i] = 0;
}
