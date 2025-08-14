#include "machine_code.h"

const bool NO_DEST_OPERAND[ADDRESSING_TYPES_AMOUNT] = {0};

word_data g_memory [MEMORY_MAX_SIZE];
int g_memory_word_index = STARTING_MEMORY_ADDRESS;

#define NO_OPERAND {0,0,0,0}

const char * function_names [] =
{
    "machine_code_main",
    "machine_code_write_machine_code",
    "machine_code_handle_instructions",
    "machine_code_handle_symbols",
    "machine_code_add_instruction_code",
    "machine_code_add_symbol_code",
    "machine_code_find_symbol",
    "machine_code_add_operand",
    "machine_code_get_operands_register",
    "machine_code_func_handler"
};

/**
 * @brief a helper functio that adds a certian amount of bits to a uint16_t after shifting its current value
 *
 * @param curr_val the uint16_t that we want to add new bits to
 * @param new_arg the new bits we want to add at the end of the curr_val
 * @param new_length the anount of the new bits
 * @return uint16_t the result of adding the new bits at the end of the existing ones
 */
static uint16_t machine_code_append_arg_to_word(uint16_t curr_val, uint16_t new_arg, int new_length)
{
    curr_val = curr_val << new_length;
    curr_val |= new_arg;
    return curr_val;
}

/**
 * @brief a helper function that is responsible that the fields of the word_content line are ordered correctly in the memmory
 *
 * @param operand_data_word the instruction operand before correct memory ordering
 * @return uint16_t the instruction operand after correct memory ordering
 */

static uint16_t machine_code_reorder_operand_word_content(operand_content operand_data_word)
{
    uint16_t ret = 0;

    /* reorder to : funct, src_reg, src_type, dest_reg, dest_type */
    ret = machine_code_append_arg_to_word(ret, operand_data_word.funct, FUNCT_MAX_BIT_SIZE);

    ret = machine_code_append_arg_to_word(ret, operand_data_word.src_register, REGISTER_MAX_BIT_SIZE);
    ret = machine_code_append_arg_to_word(ret, operand_data_word.src_operand_type, OPERAND_TYPE_MAX_BIT_SIZE);

    ret = machine_code_append_arg_to_word(ret, operand_data_word.dest_register, REGISTER_MAX_BIT_SIZE);
    ret = machine_code_append_arg_to_word(ret, operand_data_word.dest_operand_type, OPERAND_TYPE_MAX_BIT_SIZE);

    return ret;
}



void machine_code_main(symbol * symbol_list, int symbol_list_length, instruction_data * instruction_list, int instruction_list_length)
{
    memset(g_memory, 0, sizeof(g_memory));
    g_memory_word_index = STARTING_MEMORY_ADDRESS;

    machine_code_handle_instructions(symbol_list, symbol_list_length, instruction_list, instruction_list_length);
    machine_code_handle_symbols(symbol_list, symbol_list_length);
}

machine_code_status machine_code_write_machine_code(machine_code code)
{
    machine_code_status ret = MACHINE_CODE_STATUS_SUCCESS;
    if(g_memory_word_index + code.word_count >= MEMORY_MAX_SIZE)
    {
        printf("%s error: machine code not added because of memory overflow!\n", __func__);
        ret = MACHINE_CODE_STATUS_ERROR_MALLOC;
    }
    else
    {
        for(int word_index = 0; (word_index < (int)code.word_count); word_index++)
        {
            g_memory[g_memory_word_index ++] = code.words[word_index];
        }
    }
    return ret;
}

void machine_code_handle_instructions(symbol * symbol_list, int symbol_list_length, instruction_data * instruction_list, int instruction_list_length)
{
    machine_code_status adding_instruction_status;

    for(int instruction_index = 0; instruction_index < instruction_list_length; instruction_index++)
    {
        adding_instruction_status = machine_code_add_instruction_code(symbol_list, symbol_list_length, instruction_list[instruction_index]);
        if(adding_instruction_status != MACHINE_CODE_STATUS_SUCCESS)
        {
            machine_code_func_handler(adding_instruction_status, FUNC_TYPE_ADD_INSTRUCTION_CODE);
            break;
        }
    }
}

void machine_code_handle_symbols(symbol * symbol_list, int symbol_list_length)
{
    for(int symbol_index = 0; symbol_index < symbol_list_length; symbol_index++)
    {
        if(machine_code_add_symbol_code(symbol_list[symbol_index]) != MACHINE_CODE_STATUS_SUCCESS)
        {
            break;
        }
    }
}

/**
 * @brief a helper function that gets a flag array and checks if all the flags in the array are set to false
 *
 * @param flag_array the bool array to check
 * @param arr_length the length of the bool array / amount of flags to check
 * @return true in the given range all of the arrays items are set to false
 * @return false the array has at least one flag in the range of the given length that true
 *
 */
static bool flag_array_is_empty(bool flag_array[], int arr_length)
{
    bool ret = true;
    for(int i = 0; (i < arr_length) && (ret == true); i++)
    {
        ret = !flag_array[i];
    }
    return ret;
}

machine_code_status machine_code_add_instruction_code(symbol * symbol_list, int symbol_list_length, instruction_data current_instruction)
{
    machine_code_status ret = MACHINE_CODE_STATUS_SUCCESS;

    machine_code instruction_code;
    command curr_command;
    int curr_word_index = 0;
    word_data *curr_word;
    operand_content *op;

    instruction_code.word_count = current_instruction.size;
    instruction_code.words = (word_data *) malloc(sizeof(word_data) * instruction_code.word_count);
    if (!instruction_code.words)
    {
        ret = MACHINE_CODE_STATUS_ERROR_MALLOC;
    }
    else
    {
        curr_command = commands[current_instruction.command_index];

        curr_word = &instruction_code.words[curr_word_index];
        curr_word->are_attribute = ABSOLUTE;
        curr_word->content.opcode = curr_command.opcode;

        if (instruction_code.word_count > 1)
        {
            curr_word_index++;
            curr_word = &instruction_code.words[curr_word_index];
            curr_word->are_attribute = ABSOLUTE;

            op = &curr_word->content.operand;
            op->funct = curr_command.funct;
            op->dest_operand_type = current_instruction.dest_operand_data.addressing_mode;
            op->dest_register = machine_code_get_operands_register(current_instruction.dest_operand_data);
            op->src_operand_type = current_instruction.src_operand_data.addressing_mode;
            op->src_register = machine_code_get_operands_register(current_instruction.src_operand_data);
            curr_word->content.value = machine_code_reorder_operand_word_content(*op);

            curr_word_index++;

            if(!flag_array_is_empty(curr_command.src_operand_types, ADDRESSING_TYPES_AMOUNT))
            {
                curr_word_index = machine_code_add_operand(symbol_list, symbol_list_length, current_instruction.src_operand_data, &instruction_code, curr_word_index);
            }
            if(!flag_array_is_empty(curr_command.dest_operand_types, ADDRESSING_TYPES_AMOUNT))
            {
                curr_word_index = machine_code_add_operand(symbol_list, symbol_list_length, current_instruction.dest_operand_data, &instruction_code, curr_word_index);
            }
        }

        ret = machine_code_write_machine_code(instruction_code);

        if (ret != MACHINE_CODE_STATUS_SUCCESS)
        {
            machine_code_func_handler(ret, FUNC_TYPE_WRITE_MACHINE_CODE);
        }
        free(instruction_code.words);
    }
    return ret;
}

machine_code_status machine_code_add_symbol_code(symbol current_symbol)
{
    machine_code_status ret = MACHINE_CODE_STATUS_SUCCESS;

    /* check if symbol isnt external or label*/
    if(current_symbol.access_attribute != ATTRIBUTE_EXTERN && (current_symbol.size != 0) )
    {
        machine_code symbol_code;
        symbol_code.word_count = current_symbol.size;

        symbol_code.words = malloc(sizeof(word_data) * symbol_code.word_count);
        if(!symbol_code.words)
        {
            ret = MACHINE_CODE_STATUS_ERROR_MALLOC;
            printf("%s error: malloc failed\n", __func__);
        }
        else
        {
            for(int word_index = 0; word_index < (int)symbol_code.word_count; word_index++)
            {
                symbol_code.words[word_index].are_attribute =  ABSOLUTE;
                symbol_code.words[word_index].content.value = ((value_content *)current_symbol.data)[word_index];
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

static bool operand_is_empty(const operand_data *op)
{
    return (op && op->addressing_mode == 0 && op->operand_data == 0 && op->varible_name[0] == '\0');
}

int machine_code_add_operand(symbol * symbol_list, int symbol_list_length, operand_data operand, machine_code * instruction_code, int curr_word_index)
{

    if(!operand_is_empty(&operand))
    {
        are are_attribute = ABSOLUTE;
        symbol * operand_symbol;
        switch (operand.addressing_mode)
        {
        case ADDRESSING_MODES_IMMEDIATE:
            instruction_code->words[curr_word_index].are_attribute = ABSOLUTE;

            instruction_code->words[curr_word_index].content.value = operand.operand_data;
            curr_word_index++;
            break;

        case ADDRESSING_MODES_DIRECT:
        case ADDRESSING_MODES_INDEX:

            if(*operand.varible_name == "\0")
            {
                fprintf(stderr, "%s error: empty varieble name\n",__func__);
                break;
            }
            operand_symbol = machine_code_find_symbol(symbol_list,symbol_list_length, operand.varible_name);
            if(!operand_symbol)
            {
                fprintf(stderr, "%s error: non declared variabel: %s\n",__func__ , operand.varible_name);
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

            instruction_code->words[curr_word_index].content.data_address = (operand_symbol->address / 16) * 16;

            curr_word_index++;
            instruction_code->words[curr_word_index].are_attribute = are_attribute;
            instruction_code->words[curr_word_index].content.offset = operand_symbol->address % 16;
            curr_word_index++;

            break;

        case ADDRESSING_MODES_REGISTER_DIRECT:
            break;

        default:
            printf("%s: unhandled addressing mode operand: %s\n",__func__ ,operand.varible_name);
            break;
        }
    }
    return curr_word_index;

}


uint16_t machine_code_get_operands_register(operand_data operand)
{
    uint16_t ret = 0;
    if((operand.addressing_mode == ADDRESSING_MODES_REGISTER_DIRECT) || (operand.addressing_mode == ADDRESSING_MODES_INDEX))
    {
        ret = operand.operand_data;
    }
    return ret;
}


void machine_code_func_handler(machine_code_status ret, func_type func)
{
    switch (ret)//__func__ __LINE__
    {
    case MACHINE_CODE_STATUS_SUCCESS:
        /* (no error) */
        break;
    case MACHINE_CODE_STATUS_ERROR_MALLOC:
        printf("malloc error in function: %s\n", function_names[func]);
        break;
    case MACHINE_CODE_STATUS_ERROR_SYMBOL_NOT_FOUND:
        printf("symbol not found error in function: %s\n", function_names[func]);
        break;
    default:
        printf("unhandled error, in %s\n", function_names[func]);
        break;
    }

}