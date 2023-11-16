#include "stack.h"
#include "read.h"
#include "screen.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <unistd.h>


void parseinstruction(uint16_t instruction);

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
    initscr();          // ncurses setup
    for (pc = 512; pc < 4096; pc += 2) {
        // printw("%04X\n", mergeinstruction(instructions[pc], instructions[pc+1]));
        parseinstruction(mergeinstruction(instructions[pc], instructions[pc+1]));
        usleep(100000);
    }

    endwin();
    return 0;
}

void parseinstruction(uint16_t instruction)
{
    switch(instruction / 0x1000) {
        case 0x0: {
                      if (instruction == 0x00E0) {
                          clearscreen();
                      } else if (instruction == 0x00EE) {
                          pc = pop();
                      }
                      break;
                  }
        case 0x1: {
                      pc = instruction % 0x1000;
                      break;
                  }
        case 0x2: {
                      push(pc);
                      pc = instruction % 0x1000;
                      push(pc);
                      break;
                  }
        case 0x3: { // the mod could be wrong tbh
                      uint8_t x = (instruction % 0x1000) / 0x100;
                      uint16_t kk = instruction % 0x100;
                      if (gr[x] == kk)
                          pc += 2;
                      break;
                  }
        case 0x4: {
                      uint8_t x = (instruction % 0x1000) / 0x100;
                      uint16_t kk = instruction % 0x100;
                      if (gr[x] != kk)
                          pc += 2;
                      break;
                  }
        case 0x5: {
                      uint8_t x = (instruction % 0x1000) / 0x100; // this could be wrong too
                      uint8_t y = (instruction % 0x100) / 0x10;
                      if (gr[x] == gr[y])
                          pc += 2;
                      break;
                  }
        case 0x6: {
                      uint8_t x = (instruction % 0x1000) / 0x100; // this could be wrong too
                      uint16_t kk = instruction % 0x100;
                      gr[x] = kk;
                      break;
                  }
        case 0x7: {
                      uint8_t x = (instruction % 0x1000) / 0x100; // this could be wrong too
                      uint16_t kk = instruction % 0x100;
                      gr[x] += kk;
                      break;
                  }
        case 0x8: {
                      uint8_t x = (instruction % 0x1000) / 0x100;
                      uint8_t y = (instruction % 0x100) / 0x10;
                      uint8_t end = instruction % 0x10;
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
                          if (t > gr[x])
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
                          if (gr[x] / 0x80)
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
                          if (gr[x] / 0x80)
                              gr[0xF] = 1;
                          else
                              gr[0xF] = 0; gr[x] *= 2;
                      }
                      break;
                  }
        case 0x9: {
                      uint8_t x = (instruction % 0x1000) / 0x100; // this could be wrong too
                      uint8_t y = (instruction % 0x100) / 0x10;
                      if (gr[x] != gr[y])
                          pc += 2;
                      break;
                  }
        case 0xA: {
                      ir = instruction % 0x1000;
                      break;
                  }
        case 0xB: {
                      pc = gr[0] + (instruction % 0x1000);
                      break;
                  }
        case 0xC: {
                      uint8_t x = (instruction % 0x1000) / 100;
                      uint8_t kk = instruction % 0x100; 
                      gr[x] = (rand() % 255) & kk;
                      break;
                  }
        case 0xD: { // TODO fix printing this crap isn't working
                      uint8_t n = instruction % 0x10;
                      uint8_t y = (instruction % 0x100) / 0x10;
                      uint8_t x = (instruction % 0x1000) / 0x100;
                      printsprite(&instructions[ir], n, gr[x], gr[y]); // This could be broken
                      break;
                  }
        case 0xE: {
                      uint8_t end = (instruction % 0x100);
                      uint8_t x = (instruction % 0x1000) / 0x100;
                      uint32_t key = getch();
                      if (end == 0x9E && key == gr[x]) {
                          pc += 2; 
                      } else if (end == 0xA1 && key != gr[x]) {
                          pc += 2;
                      }
                      break;
                  }
        case 0xF: {
                    break;
                    // will implement this later
                  }
    }
}
