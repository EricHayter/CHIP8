#include <stdint.h>
#include <stdio.h>
#define STACKSIZE 16

static uint8_t sp = 0;
static uint16_t stack[16];

void push(uint16_t val) {
	if (sp >= STACKSIZE) {
		printf("error: stack is full");
		return;
	}
	stack[sp++] = val;
}

uint16_t pop()
{
	if (sp == 0) {
		printf("error: stack is empty");
		return 0;
	}
	return stack[--sp];
}

uint8_t size()
{
    return sp;
}
