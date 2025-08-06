#include "machine_code.h"

const bool NO_DEST_OPERAND[ADDRESSING_TYPES_AMONT] = {0};

int g_memory_word_index = STARTING_MEMORY_ADDRESS;
word_data g_memory [MEMORY_MAX_SIZE];


command * machine_code_get_command(char * command_name)
{
    command * ret = NULL;
    for(int i = 0; (i < COMMAND_AMONT) && (!ret); i++)
    {
        if(strncmp(commands[i].command_name, command_name, COMMAND_MAX_SIZE) == 0)
        {
            ret = &commands[i];
        }
    }

    return ret;
}

void machine_code_main(symbol * symbol_list, int symbol_list_length, instruction * instruction_list, int instruction_list_length)
{

    memset(g_memory, 0, sizeof(g_memory));
    if(g_memory)
    {
        perror("machine code allocating memory error\n");
        exit(1);
    }
    machine_code_handle_instructions(symbol_list, instruction_list, instruction_list_length);
}

void machine_code_add_last_word(machine_code code)
{
    g_memory[g_memory_word_index ++] = code.words[code.word_count - 1];
}

void machine_code_handle_instructions(symbol * symbol_list, instruction * instruction_list, int instruction_list_length)
{
    instruction current_instruction;
    machine_code curr_code;
    command curr_command;
    word_data curr_word;

    for(int instruction_index = 0; instruction_index < instruction_list_length; instruction_index++)
    {
        /* every instruction consists of atleast 1 word */
        curr_code.word_count = 1;

        current_instruction = instruction_list[instruction_index];
        curr_command = * machine_code_get_command(current_instruction.commnand);
        curr_code.words->are_attribute = ABSOLUTE;
        curr_code.words->content.opcode = curr_command.opcode;

        machine_code_add_last_word(curr_code);

        /* check if this action has operands or not */
        if(memcmp(curr_command.dest_operand_types, NO_DEST_OPERAND, ADDRESSING_TYPES_AMONT) == 0)
        {
            continue;
        }

        curr_code.word_count++;
        curr_code.words[curr_code.word_count - 1].
    }
}
