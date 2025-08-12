#include "machine_code.h"
#include <stdio.h>
#include <string.h>

#ifndef STOP_CMD_INDEX
#define STOP_CMD_INDEX 0
#endif

static void print_machine_code(int amount);
static void print_word(word_data w);

int main(void)
{
    value_content k_storage[1] = { 31 };

    symbol symbols[1] = {0};
    symbols[0].access_attribute=0;
    symbols[0].address = 105;
    symbols[0].data = k_storage;
    symbols[0].data_attribute = ATTRIBUTE_DATA;
    strcpy(symbols[0].name, "k");
    symbols[0].size = 1;

    instruction_data ins[1];
    memset(&ins[0], 0, sizeof(ins[0]));
    ins[0].size = 4;                 /* one word (opcode only) */
    ins[0].command_index = 7;
    ins[0].address = 101;
    ins[0].dest_operand_data.addressing_mode = ADDRESSING_MODES_DIRECT;
    strcpy(ins[0].dest_operand_data.varible_name, "k");
    ins[0].src_operand_data = (operand_data){0};

    machine_code_main(symbols, 1, ins, 1);

    print_machine_code(6);             /* expect exactly 1 word */
    return 0;
}

static void print_machine_code(int amount)
{
    for (int i = 0; i < amount; i++)
    {
        print_word(g_memory[STARTING_MEMORY_ADDRESS + i]);
    }
}
#ifndef ARE_BITS
#define ARE_BITS 4
#endif

#ifndef VALUE_BITS
#define VALUE_BITS 16
#endif

static void print_bits_u(unsigned v, int width)
{
    for (int i = width - 1; i >= 0; --i)
    {
        putchar( (v >> i) & 1 ? '1' : '0' );
        if(i % 4 == 0 && i != 0)
        {
            putchar('.');
        }
    }
}
#include <stdint.h>

static void print_word(word_data w)
{
    unsigned are = (unsigned)w.are_attribute;
    uint16_t val16 = (uint16_t)w.content.value;  // truncates to 16 bits without sign-ext
    unsigned val = (unsigned)val16;

    printf("ARE=");
    print_bits_u(are, ARE_BITS);
    printf(" VALUE=");
    print_bits_u(val, VALUE_BITS);
    putchar('\n');

    printf("ARE=%u VALUE=%u\n", are, val);
}


