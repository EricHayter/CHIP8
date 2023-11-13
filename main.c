#include "stack.h"
#include "read.h"
#include "screen.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>


void parseinstruction(uint16_t *instruction);

uint8_t gr[16]; // general purpose register
uint16_t ir;    // "I" register
uint8_t sr, dr; // sound and delay registers
int16_t pc = 0x0;   // program counter (TODO check if this needs an offset)
                    //



int main(int argc, char **argv)
{
    srand(time(NULL));
    initscr();
    printw("Hello world!");
    refresh();
    uint8_t key = getch();
    printw(" %d", key);
    getch();
    endwin();
    // uint16_t *instructionpointer;
    // char *filename = argv[1];
    // uint16_t instructions[INSTRUCTION_SIZE]; // 0-255 values
    // if (readfile(filename, instructions)) // reading failed
    //     return 1;
    // for (int i = 0; i < 20; i++) {
    //     printf("%04X\n", instructions[i]);
    // }

    // instructionpointer = &instructions[0]; 
    return 0;
}

void parseinstruction(uint16_t *instruction)
{
    switch(*instruction / 0x1000) {
        case 0x0: {
                      if (*instruction == 0x00E0) {
                          clearscreen();
                      } else if (*instruction == 0x00EE) {
                          // return to the top of the stack
                      }
                      break;
                  }
        case 0x1: {
                      pc = *instruction % 0x1000;
                      break;
                  }
        case 0x2: {
                      push(pc);
                      pc = *instruction % 0x1000;
                      push(pc);
                      break;
                  }
        case 0x3: { // the mod could be wrong tbh
                      uint8_t x = (*instruction % 0x1000) / 0x100;
                      uint16_t kk = *instruction % 0x100;
                      if (gr[x] == kk)
                          pc += 2;
                      break;
                  }
        case 0x4: {
                      uint8_t x = (*instruction % 0x1000) / 0x100;
                      uint16_t kk = *instruction % 0x100;
                      if (gr[x] != kk)
                          pc += 2;
                      break;
                  }
        case 0x5: {
                      uint8_t x = (*instruction % 0x1000) / 0x100; // this could be wrong too
                      uint8_t y = (*instruction % 0x100) / 0x10;
                      if (gr[x] == gr[y])
                          pc += 2;
                      break;
                  }
        case 0x6: {
                      uint8_t x = (*instruction % 0x1000) / 0x100; // this could be wrong too
                      uint16_t kk = *instruction % 0x100;
                      gr[x] = kk;
                      break;
                  }
        case 0x7: {
                      uint8_t x = (*instruction % 0x1000) / 0x100; // this could be wrong too
                      uint16_t kk = *instruction % 0x100;
                      gr[x] += kk;
                      break;
                  }
        case 0x8: {
                      uint8_t x = (*instruction % 0x1000) / 0x100;
                      uint8_t y = (*instruction % 0x100) / 0x10;
                      uint8_t end = *instruction % 0x10;
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
                              gr[0xF] = 0;
                          gr[x] *= 2;
                      }
                      break;
                  }
        case 0x9: {
                      uint8_t x = (*instruction % 0x1000) / 0x100; // this could be wrong too
                      uint8_t y = (*instruction % 0x100) / 0x10;
                      if (gr[x] != gr[y])
                          pc += 2;
                      break;
                  }
        case 0xA: {
                      ir = *instruction % 0x1000;
                      break;
                  }
        case 0xB: {
                      pc = gr[0] + (*instruction % 0x1000);
                      break;
                  }
        case 0xC: {
                      uint8_t x = (*instruction % 0x1000) / 100;
                      uint8_t kk = *instruction % 0x100; 
                      gr[x] = (rand() % 255) & kk;
                      break;
                  }
        case 0xD: {
                      // do this one later
                  }
        case 0xE: {
                      uint8_t end = (*instruction % 100);
                      if (end == 0x9E) {
                          // check the keyboard state
                      } else if (end == 0xA1) {
                          // check the keybaord state
                      }
                      break;
                  }
        case 0xF: {

                  }
    }
}
