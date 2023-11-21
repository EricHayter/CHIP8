#include "stack.h"
#include "read.h"
#include "screen.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <unistd.h>

// TODO debugging on the brix game, currently getting stunlocked on pc=564 and keep circling
// on the same point. For some reason the value of V[0] needs to be 0 but isn't when read
// PC 560 we set to 64????????????????????
// we aren't printing correctly for some reason?
// errasing the bars in brix.ch8

void parseinstruction(uint8_t inststrt, uint8_t instend);

uint8_t gr[16]; // general purpose register
uint16_t ir;    // "I" register
uint8_t sr, dr; // sound and delay registers
uint8_t instructions[4096];
int16_t pc;   // program counter (TODO check if this needs an offset)

int main(int argc, char **argv)
{
    char *filename = argv[1];
    if (readfile(filename, instructions)) {
        return 1;
    }

    srand(time(NULL));  // RNG for program
    initscr();
    noecho();           // ncurses setup stuff
    cbreak();
    timeout(0);

    for (pc = 512; pc < 4096; pc += 2) {
        parseinstruction(instructions[pc], instructions[pc+1]);
        usleep(5000);
    }

    endwin();
    return 0;
}


void parseinstruction(uint8_t instrstart, uint8_t instrend)
{
    switch(instrstart / 0x10) {
        case 0x0: {
                      if (instrstart == 0x00 && instrend == 0xE0) {
                          clearscreen();
                      } else if (instrstart == 0x00 && instrend == 0xEE) {
                          pc = pop();
                      }
                      break;
                  }
        case 0x1: {
                      pc = mergeinstruction(instrstart % 0x10, instrend);
                      break;
                  }
        case 0x2: {
                      push(pc);
                      pc = mergeinstruction(instrstart % 0x10, instrend);
                      break;
                  }
        case 0x3: { 
                      uint8_t x = instrstart % 0x10;
                      if (gr[x] == instrend)
                          pc += 2;
                      break;
                  }
        case 0x4: {
                      uint8_t x = instrstart % 0x10;
                      if (gr[x] != instrend)
                          pc += 2;
                      break;
                  }
        case 0x5: {
                      uint8_t x = instrstart % 0x10;
                      uint8_t y = instrend / 0x10;
                      if (gr[x] == gr[y])
                          pc += 2;
                      break;
                  }
        case 0x6: {
                      uint8_t x = instrstart % 0x10;
                      gr[x] = instrend;
                      break;
                  }
        case 0x7: {
                      uint8_t x = instrstart % 0x10;
                      gr[x] += instrend;
                      break;
                  }
        case 0x8: {
                      uint8_t x = instrstart % 0x10;
                      uint8_t y = instrend / 0x10;
                      uint8_t end = instrend % 0x10;
                      if (end == 0x0) {
                          gr[x] = gr[y];
                      } else if (end == 0x1) {
                          gr[x] = gr[x] | gr[y];
                      } else if (end == 0x2) {
                          gr[x] = gr[x] & gr[y];
                      } else if (end == 0x3) {
                          gr[x] = gr[x] ^ gr[y];
                      } else if (end == 0x4) {
                          uint8_t t = gr[x];  
                          gr[x] = gr[x] + gr[y];
                          if (t > gr[x]) // original value is bigger than current (overflow)
                              gr[0xF] = 1;
                          else 
                              gr[0xF] = 0;
                      } else if (end == 0x5) {
                          if (gr[x] > gr[y])
                              gr[0xF] = 1;
                          else 
                              gr[0xF] = 0;
                          gr[x] -= gr[y];
                      } else if (end == 0x6) {
                          if (gr[x] % 0x10)
                              gr[0xF] = 1;
                          else
                              gr[0xF] = 0;
                          gr[x] /= 2;
                      } else if (end == 0x7) {
                          if (gr[y] > gr[x])
                              gr[0xF] = 1;
                          else
                              gr[0xF] = 0;
                          gr[x] -= gr[y];
                      } else if (end == 0xE) {
                          if (gr[x] >> 7)
                              gr[0xF] = 1;
                          else
                              gr[0xF] = 0; 
                          gr[x] *= 2;
                      }
                      break;
                  }
        case 0x9: {
                      uint8_t x = instrstart % 0x10;
                      uint8_t y = instrend / 0x10;
                      if (gr[x] != gr[y])
                          pc += 2;
                      break;
                  }
        case 0xA: {
                      ir = mergeinstruction(instrstart % 0x10, instrend);
                      break;
                  }
        case 0xB: {
                      pc = (uint16_t)(gr[0]) + mergeinstruction(instrstart % 0x10, instrend);
                      break;
                  }
        case 0xC: {
                      uint8_t x = instrstart % 0x10;
                      gr[x] = (rand() % 255) & instrend;
                      break;
                  }
        case 0xD: {
                      uint8_t n = instrend % 0x10;
                      uint8_t y = instrend / 0x10;
                      uint8_t x = instrstart % 0x10;
                      gr[0xF] = printsprite(&instructions[ir], n, gr[x], gr[y]);
                      break;
                  }
        case 0xE: {
                      uint8_t end = instrend;
                      uint8_t x = instrstart % 0x10;
                      uint32_t key = getch();
                      if (end == 0x9E && getch() == gr[x]) {
                          pc += 2; 
                      } else if (end == 0xA1 && getch() != gr[x]) {
                          pc += 2;
                      }
                      break;
                  }
        case 0xF: {
                      uint8_t x = instrstart % 0x10;
                      if (instrend == 0x07) {
                          gr[x] = dr;
                      } else if (instrend == 0x0A) {
                          timeout(-1);
                          gr[x] = getch();
                          timeout(0);
                      } else if (instrend == 0x15) {
                          dr = gr[x];
                      } else if (instrend == 0x18) {
                          sr = gr[x];
                      } else if (instrend == 0x1E) {
                          ir += gr[x];
                    } else if (instrend == 0x29) {
                        ir = (uint16_t)(gr[x]) * 5;
                      } else if (instrend == 0x33) {
                          instructions[ir] = (x % 1000) / 100;
                          instructions[ir+1] = (x % 100) / 10;
                          instructions[ir+2] = x % 10;
                      } else if (instrend == 0x55) {
                          for (uint8_t i = 0; i < x; i++) {
                              instructions[ir+i] = gr[i];
                          }
                      } else if (instrend == 0x65) {
                          for (uint8_t i = 0; i < x; i++) {
                              gr[i] = instructions[ir+i];
                          }
                      }
                      break;
                  }
    }
}
