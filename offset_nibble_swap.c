// (C) anayae 2024

#include <stdio.h>
#include <string.h>

typedef struct byte_s
{
    unsigned nibble0: 4;
    unsigned nibble1: 4;
} byte;

int main(int argc, char* argv[])
{
    if (argc < 1) return 1;

    printf("out:\n");

    size_t size = strlen(argv[1]);
    for (int i = 0; i < size; i += 2)
    {
        byte both;

        sscanf(argv[1] + i, "%2x", &both); // reading two nibbles: 0x_ and 0x_ from string

        both.nibble0 = 0xF - both.nibble0; // GUYS ITS A KEY PAIR GUYS ITS NOT SUBTRACTION !!! - TANISH
        both.nibble1 = 0xF - both.nibble1; // (it was subtraction)

        // other part: nibble swap.
        byte old = both;
        both.nibble0 = both.nibble1;
        both.nibble1 = old.nibble0;

        printf("%c", *((char*)&both));
    }
    printf("\n");
}