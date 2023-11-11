#include "screen.h"
#include <stdint.h>
#include <stdio.h>

static uint64_t screen[32]; 
                     
void printscreen()
{
    for (int i = 0; i < 32; i++) {
        uint64_t cp = screen[i]; 
        while (cp) {
            if (cp >> 63 && 1)
                printf("#");
            else
                printf(" ");
        }
        printf("\n");
    }
}

void clearscreen()
{
    for (int i = 0; i < 32; i++)
        screen[i] = 0;
}
