#include "stack.h"
#include "read.h"
#include "screen.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <unistd.h>
#include <pthread.h>


void parseinstruction(uint8_t inststrt, uint8_t instend);
void *readinput();

uint8_t gr[16]; // general purpose register
uint16_t ir;    // "I" register
uint8_t sr, dr; // sound and delay registers
uint8_t instructions[4096];
int16_t pc;   // program counter (TODO check if this needs an offset)
uint8_t input;
uint8_t blockinginput = 0; // some input requires blocking

int main(int argc, char **argv)
{
    char *filename = argv[1];
    if (readfile(filename, instructions)) {
        return 1;
    }

    srand(time(NULL));  // RNG for program
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, readinput, NULL);
    initscr();          // ncurses setup
    for (pc = 512; pc < 4096; pc += 2) {
        parseinstruction(instructions[pc], instructions[pc+1]);
        usleep(100000);
    }

    endwin();
    return 0;
}

// async function to gather input for games
void *readinput() {
    while (1) {
        input = getch(); 
        if (blockinginput != 0) {
            blockinginput = 0;
        }
    }
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
                      push(pc);
                      break;
                  }
        case 0x3: { // the mod could be wrong tbh
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
                      uint8_t x = instrstart % 0x10; // this could be wrong too
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
                      pc = gr[0] + mergeinstruction(instrstart % 0x10, instrend);
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
                      printsprite(&instructions[ir], n, gr[x], gr[y]); // This could be broken
                      break;
                  }
        case 0xE: {
                      uint8_t end = instrend;
                      uint8_t x = instrstart % 0x10;
                      uint32_t key = getch();
                      if (end == 0x9E && key == gr[x]) {
                          pc += 2; 
                      } else if (end == 0xA1 && key != gr[x]) {
                          pc += 2;
                      }
                      break;
                  }
        case 0xF: {
                      uint8_t x = instrstart % 0x10;
                      if (instrend == 0x07) {
                          gr[x] = dr;
                      } else if (instrend == 0x0A) {
                        blockinginput = 1;
                        while (blockinginput) // this might be blocking forever not really sure
                            ;
                        gr[x] = input;
                      } else if (instrend == 0x15) {
                        dr = gr[x];
                      } else if (instrend == 0x18) {
                          sr = gr[x];
                      } else if (instrend == 0x1E) {
                        ir += gr[x]; 
                      }
                      break;
                      // will implement this later
                  }
    }
}
