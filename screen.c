#include "screen.h"
#include <stdint.h>
#include <stdio.h>
#include <curses.h>

static uint64_t screen[32]; 
                     
void printscreen()
{
    clear();
    for (int i = 0; i < 32; i++) {
        uint64_t cp = screen[i]; 
        for (int j = 63; j >= 0; j--) {
            if ((cp >> j) & 1)
                printw("#");
            else
                printw(" ");
        }
        printw("\n");
    }
    refresh();
}

void printsprite(uint8_t *sprite, uint8_t size, uint8_t x, uint8_t y) {
    for (uint8_t i = 0; i < size; i++) {
        loadbytetoscreen(*(sprite+i), x, y+i);
    }
    printscreen();
}

void loadbytetoscreen(uint8_t b, uint8_t x, uint8_t y) {
    uint64_t nb = (uint64_t)(b);
    if (x <= 56) {
        nb <<=  64-8 - x;
    } else {
        nb = nb;
    }
    screen[y] = nb ^ screen[y];   
}

void clearscreen()
{
    for (int i = 0; i < 32; i++)
        screen[i] = 0;
}
