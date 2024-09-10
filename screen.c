#include "screen.h"
#include <stdint.h>
#include <stdio.h>
#include <curses.h>

static uint64_t screen[32]; 
                     
void printscreen()
{
    clear();

	// print top border
	for (int i = 0; i < 64 + 2; i++)
		printw("*");
	printw("\n");
	
    for (int i = 0; i < 32; i++) {
		// print side border
		printw("*");
        for (int j = 63; j >= 0; j--) {
            if ((screen[i] >> j) & 1)
                printw("#");
            else
                printw(" ");
        }
		
		// print side border
		printw("*");
        printw("\n");
    }

	// print bottom border
	for (int i = 0; i < 64 + 2; i++)
		printw("*");
	printw("\n");

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
    x = x % 64;
    uint64_t shiftedvalue;
    if (x + sizeof(b)*8 >= 64) {
        shiftedvalue = ((uint64_t)b) << (64 - x - sizeof(b)*8);
        shiftedvalue |= ((uint64_t)b) >> (x + sizeof(b)*8 - 64);
    } else {
        shiftedvalue = ((uint64_t)b) << (64 - x - sizeof(b)*8);
    }
    uint8_t change = (screen[y] & ~shiftedvalue) > 0;
    screen[y] = screen[y] ^ shiftedvalue;
    return change;
}

void clearscreen()
{
    for (int i = 0; i < 32; i++)
        screen[i] = 0;
}
