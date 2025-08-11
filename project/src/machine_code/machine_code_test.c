#include "machine_code.h"

int main (void)
{
    symbol symbol_list[3];
    instruction instructions[1] =
    {
        {.commnand= "stop", .dest_operand = NULL, .src_operand= NULL, .label=NULL}
    };
    machine_code_main(symbol_list, 3, instructions, 1);
}

void print_machine_code(int amount)
{
    for (int i = 0; i < amount ; i++)
    {
        print_word(g_memory[i]);
    }
}

void print_word(word_data word)
{
    for(int i = 0; i < 4; i++)
    {
        printf("%d", (word << 1));
    }
    for(int i = 0; i < 16; i++)
    {
        printf("%d", (word << 1));
    }
    printf("\n");
}