#include "machine_code.h"

const bool NO_DEST_OPERAND[ADDRESSING_TYPES_AMONT] = {0};

int g_memory_word_index = STARTING_MEMORY_ADDRESS;

void machine_code_main(symbol * symbol_list, int symbol_list_length, instruction * instruction_list, int instruction_list_length)
{

    memset(g_memory, 0, sizeof(g_memory));
    if(g_memory)
    {
        perror("machine code allocating memory error\n");
        exit(1);
    }
    machine_code_handle_instructions(symbol_list, instruction_list, instruction_list_length, instruction_list_length);

}

status machine_code_write_machine_code(machine_code code)
{
    status ret = SUCCESS;
    if(g_memory_word_index + code.word_count >= MEMORY_MAX_SIZE)
    {
        printf("%s error: machine code not added because of memory overflow!\n", __func__);
        ret = FAILURE;
    }
    for(int word_index = 0; (word_index < code.word_count); word_index++)
    {
        g_memory[g_memory_word_index ++] = code.words[word_index];
    }
    return ret;
}

void machine_code_handle_instructions(symbol * symbol_list, int symbol_list_length, instruction_data * instruction_list, int instruction_list_length)
{
    for(int instruction_index = 0; instruction_index < instruction_list_length; instruction_index++)
    {
        if(machine_code_add_instruction_code(symbol_list, symbol_list_length, instruction_list[instruction_index]) == FAILURE)
        {
            break;
        }
    }
}

void machine_code_handle_symbols(symbol * symbol_list, int symbol_list_length)
{
    for(int symbol_index = 0; symbol_index < symbol_list_length; symbol_index++)
    {
        if(machine_code_add_symbol_code(symbol_list[symbol_index]) == FAILURE)
        {
            break;
        }
    }
}

status machine_code_add_instruction_code(symbol * symbol_list, int symbol_list_length, instruction_data current_instruction)
{
    status ret = SUCCESS;

    machine_code instruction_code;
    command curr_command;
    int curr_word_index = 0;
    word_data curr_word;
    operand_content op;


    instruction_code.word_count = current_instruction.size;
    instruction_code.words = (word_data *) malloc(sizeof(word_data) * instruction_code.word_count);
    if(!instruction_code.words)
    {
        printf("%s error: malloc failed\n", __func__);
        ret = FAILURE;
    }
    else
    {
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
            curr_word_index = machine_code_add_operand(symbol_list,symbol_list_length,current_instruction.src_operand_data,&instruction_code,curr_word_index);
            curr_word_index = machine_code_add_operand(symbol_list,symbol_list_length,current_instruction.dest_operand_data,&instruction_code,curr_word_index);
        }
        ret = machine_code_write_machine_code(instruction_code);
        free(instruction_code.words);
    }
    return ret;
}

status machine_code_add_symbol_code(symbol current_symbol)
{
    status ret = SUCCESS;
    if(current_symbol.access_attribute != ATTRIBUTE_EXTERN && (current_symbol.data_attribute == ATTRIBUTE_DATA || current_symbol.data_attribute == ATTRIBUTE_STRING) )
    {
        machine_code symbol_code;
        symbol_code.word_count = current_symbol.data_length;

        symbol_code.words = malloc(sizeof(word_data) * symbol_code.word_count);
        if(!symbol_code.words)
        {
            ret = FAILURE;
            printf("%s error: malloc failed\n", __func__);
        }
        else
        {
            for(int word_index = 0; word_index < symbol_code.word_count; word_index++)
            {
                symbol_code.words[word_index].are_attribute = ABSOLUTE;
                symbol_code.words[word_index].content.value;
            }
            machine_code_write_machine_code(symbol_code);
            free(symbol_code.words);
        }
    }
    return ret;
}
symbol* machine_code_find_symbol(symbol * symbol_list, int symbol_list_length, const char * symbol_name)
{
    symbol * ret = NULL;
    for(int symbol_index = 0; (!ret) && (symbol_index < symbol_list_length); symbol_index++)
    {
        if(strncmp(symbol_list[symbol_index].name, symbol_name, SYMBOL_MAX_SIZE) == 0)
        {
            ret = &symbol_list[symbol_index];
        }

    }
    return ret;
}

int machine_code_add_operand(symbol * symbol_list, int symbol_list_length, operand_data operand, machine_code * instruction_code, int curr_word_index)
{
    switch (operand.operand_mode)
    {
    case ADDRESSING_MODES_IMMEDIATE:
        instruction_code->words[curr_word_index].are_attribute = ABSOLUTE;

        instruction_code->words[curr_word_index].content.value = operand.operand_data;
        curr_word_index++;
        break;

    case ADDRESSING_MODES_DIRECT:
    case ADDRESSING_MODES_INDEX:

        are are_attribute;

        symbol * operand_symbol = machine_code_find_symbol(symbol_list,symbol_list_length, operand.varible_name);
        if(!operand_symbol)
        {
            printf("%s error: non declared variabel: %s\n",__func__ , operand.varible_name);
            break;
        }
        if(operand_symbol->access_attribute == ATTRIBUTE_EXTERN)
        {
            are_attribute = EXTERNAL;
        }
        else
        {
            are_attribute = RELOCATABLE;
        }
        instruction_code->words[curr_word_index].are_attribute = are_attribute;

        instruction_code->words[curr_word_index].content.data_address = operand_symbol->base_address;

        curr_word_index++;
        instruction_code->words[curr_word_index].are_attribute = are_attribute;
        instruction_code->words[curr_word_index].content.offset = operand_symbol->offset;
        curr_word_index++;

        break;

    case ADDRESSING_MODES_REGISTER_DIRECT:
        break;

    default:
        printf("%s: unhandled addressing mode operand: %s\n",__func__ ,operand.varible_name);
        break;
    }
    return curr_word_index;
}


uint16_t machine_code_get_operands_register(operand_data operand)
{
    uint16_t ret = 0;
    if((operand.operand_mode == ADDRESSING_MODES_REGISTER_DIRECT) || (operand.operand_mode == ADDRESSING_MODES_INDEX))
    {
        ret = operand.operand_data;
    }
    return ret;
}


