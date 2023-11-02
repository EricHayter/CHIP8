#include <stdio.h>
#include <stdint.h>

typedef uint8_t sprite[5];

void print_sprite(sprite s);

sprite six = {0xF0, 0x80, 0xF0, 0x80, 0xF0};

uint8_t vm_register[16];
uint16_t stack[16];
uint8_t top = 0;

void print_sprite(sprite s)
{
    uint8_t temp;
    for (int i = 0; i < 5; i++)
    {
        temp = s[i];
        while (temp)
        {
            if (temp & 0x80)
                printf("*");
            else
                printf(" ");
            temp <<= 1;
        }
        printf("\n");
    }
}

int main ()
{
    printf("Hello world!\n");
    print_sprite(six);
    printf("%d", six[3]);

    return 0;
}

