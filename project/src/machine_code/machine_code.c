#include "machine_code.h"

const bool NO_DEST_OPERAND[ADDRESSING_TYPES_AMONT] = {0};

int g_memory_word_index = STARTING_MEMORY_ADDRESS;
word_data g_memory [MEMORY_MAX_SIZE];

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

void machine_code_handle_instructions(symbol * symbol_list, instruction_data * instruction_list, int instruction_list_length)
{
    instruction_data current_instruction;
    machine_code curr_code;
    command curr_command;
    word_data curr_word;



    for(int instruction_index = 0; instruction_index < instruction_list_length; instruction_index++)
    {
        curr_code.word_count = current_instruction.size;

        current_instruction = instruction_list[instruction_index];
        curr_command = commands[current_instruction.command_index];

        curr_code.words->are_attribute = ABSOLUTE;
        curr_code.words->content.opcode = curr_command.opcode;

        machine_code_add_last_word(curr_code);

        /* check if this action has operands or not */
        if(memcmp(curr_command.dest_operand_types, NO_DEST_OPERAND, ADDRESSING_TYPES_AMONT) == 0)
        {
            continue;
        }

        curr_code.word_count++;
        curr_word = curr_code.words[curr_code.word_count - 1];
        curr_word.content.operand.funct = curr_command.funct;
        curr_word.content.operand.dest_register = current_instruction.dest_operand;
        curr_word.content.operand.dest_operand_type = current_instruction.dest_operand;
    }
}

void machine_code_add_instruction_code(symbol * symbol_list, instruction_data current_instruction)
{
    machine_code instruction_code;
    command curr_command;
    int curr_word_index = 0;
    word_data curr_word;
    operand_content op;


    instruction_code.word_count = current_instruction.size;

    curr_command = commands[current_instruction.command_index];

    instruction_code.words[curr_word_index].are_attribute = ABSOLUTE;
    instruction_code.words[curr_word_index].content.opcode = curr_command.opcode;

    /* check if this action has operands or not */
    if(instruction_code.word_count > 1)
    {
        curr_word_index++;
        curr_word = instruction_code.words[curr_word_index];
        curr_word.are_attribute = ABSOLUTE;

        op = curr_word.content.operand;
        op.funct = curr_command.funct;
        op.dest_operand_type = current_instruction.dest_operand_data.operand_mode;
        op.dest_register = machine_code_get_operands_register(current_instruction.dest_operand_data);
        op.src_operand_type = current_instruction.src_operand_data.operand_mode;
        op.src_register = machine_code_get_operands_register(current_instruction.src_operand_data);

        curr_word_index++;
        curr_word = instruction_code.words[curr_word_index];

    }


}

int machine_code_add_operand(symbol * symbol_list, operand_data operand, machine_code * instruction_code, int curr_word_index)
{
    switch (operand.operand_mode)
    {
    case ADDRESSING_MODES_IMMEDIATE:
        /* code */
        break;

    case ADDRESSING_MODES_DIRECT:
        /* code */
        break;

    case ADDRESSING_MODES_INDEX:
        /* code */
        break;

    case ADDRESSING_MODES_REGISTER_DIRECT:
        /* code */
        break;

    default:
        break;
    }
}

/**
 * @brief the function gets an operand and returns what needs to be stored in it's corresponding register
 *
 * @param operand the operand that we want to check its mode and register
 * @return uint16_t if the operands mode uses a register return the register(the operand data), otherwise retrun 0
 */
uint16_t machine_code_get_operands_register(operand_data operand)
{
    uint16_t ret = 0;
    if((operand.operand_mode == ADDRESSING_MODES_REGISTER_DIRECT) || (operand.operand_mode == ADDRESSING_MODES_INDEX))
    {
        ret = operand.operand_data;
    }
    return ret;
}


