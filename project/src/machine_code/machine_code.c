#include "machine_code.h"
#include "out_files.h"

const bool NO_DEST_OPERAND[ADDRESSING_TYPES_AMOUNT] = {0};

word_data g_memory [MEMORY_MAX_SIZE];
int g_memory_word_index = STARTING_MEMORY_ADDRESS;

/* a table of symbol calls (symbol name and address) with all the calls for external varieables that is passed to the out_files module*/
symbol_call * g_externals = NULL;

/* a table of symbol calls (symbol name and address) with all the defenitions for entry varieables that is passed to the out_files module*/
symbol_call * g_entrys = NULL;

int g_extern_call_length = 0;
int g_entry_defenition_length = 0;

int g_instructions_word_count = 0;
int g_symbols_word_count = 0;

bool g_machine_code_error_flag = false;

/**
 * @brief a helper functio that adds a certian amount of bits to a uint16_t after shifting its current value
 *
 * @param curr_val the uint16_t that we want to add new bits to
 * @param new_arg the new bits we want to add at the end of the curr_val
 * @param new_length the anount of the new bits
 * @return uint16_t the result of adding the new bits at the end of the existing ones
 */
static inline uint16_t machine_code_append_arg_to_word(uint16_t curr_val, uint16_t new_arg, int new_length)
{
    curr_val = curr_val << new_length;
    uint16_t mask = 0xFF;
    for(int i = 0; i < (15 - new_length); i++)
    {
        mask = mask >> 1;
    }
    curr_val |= (new_arg & mask);
    return curr_val;
}

/**
 * @brief a helper function that is responsible that the fields of the word_content line are ordered correctly in the memmory
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

/**
 * @brief a helper function that adds a symbol_call item to the apropriate global table (g_externals table or g_entrys table)
 *
 * @param new_item the symbol_call to add to one of the tables
 * @param type the type of the new item that determins to wich table to add the item to
 * @param func_name a string that is used to return this functions name to the caller function
 * @return machine_code_status status that represents wether the function was executed successfully or with wich errors
 */
static machine_code_status add_item(symbol_call new_item, enum attribute_access_type_e type, const char ** func_name)
{
    *func_name = __func__;

    symbol_call **array;
    int *array_length;
    machine_code_status ret = MACHINE_CODE_STATUS_SUCCESS;

    if (type == ATTRIBUTE_EXTERN)
    {
        array = &g_externals;
        array_length = &g_extern_call_length;
    }
    else if(type == ATTRIBUTE_ENTERY)
    {
        array = &g_entrys;
        array_length = &g_entry_defenition_length;
    }
    else
    {

        ret = MACHINE_CODE_STATUS_ERROR_UNKNOWN_ATTRIBUTE_ACCESS_TYPE;
    }

    if(ret == MACHINE_CODE_STATUS_SUCCESS)
    {

        symbol_call *temp = realloc(*array, (*array_length + 1) * sizeof (**array));
        if (!temp)
        {

            ret = MACHINE_CODE_STATUS_ERROR_REALLOC;
        }
        else
        {
            *array = temp;
            (*array)[*array_length] = new_item;
            (*array_length)++;
        }
    }
    return ret;
}


bool machine_code_main(char * base_file_name,symbol * symbol_list, int symbol_list_length, instruction_data * instruction_list, int instruction_list_length)
{
    memset(g_memory, 0, sizeof(g_memory));
    g_memory_word_index = STARTING_MEMORY_ADDRESS;

    g_externals = NULL;
    g_extern_call_length = 0;
    g_entrys = NULL;
    g_entry_defenition_length = 0;


    machine_code_handle_instructions(symbol_list, symbol_list_length, instruction_list, instruction_list_length);
    machine_code_handle_symbols(symbol_list, symbol_list_length);

    if(g_machine_code_error_flag == true)
    {
        out_files_main(g_instructions_word_count, g_symbols_word_count, base_file_name);
    }

    free(g_entrys);
    free(g_externals);

    return g_machine_code_error_flag == false;
}

machine_code_status machine_code_write_machine_code(machine_code code, const char ** func_name)
{
    *func_name = __func__;

    machine_code_status ret = MACHINE_CODE_STATUS_SUCCESS;
    if(g_memory_word_index + code.word_count > MEMORY_MAX_SIZE)
    {

        ret = MACHINE_CODE_STATUS_ERROR_MEMORY_OVERFLOW;
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
    const char * child_func_name;
    machine_code_status adding_instruction_status;

    for(int instruction_index = 0; instruction_index < instruction_list_length; instruction_index++)
    {
        adding_instruction_status = machine_code_add_instruction_code(symbol_list, symbol_list_length, instruction_list[instruction_index], &child_func_name);
        if(adding_instruction_status != MACHINE_CODE_STATUS_SUCCESS)
        {
            machine_code_func_handler(adding_instruction_status, child_func_name);
            break;
        }
    }
}

void machine_code_handle_symbols(symbol * symbol_list, int symbol_list_length)
{
    const char * child_func_name;
    machine_code_status child_status;
    for(int symbol_index = 0; symbol_index < symbol_list_length; symbol_index++)
    {
        child_status = machine_code_add_symbol_code(symbol_list[symbol_index], &child_func_name);
        machine_code_func_handler(child_status, child_func_name);
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

machine_code_status machine_code_add_instruction_code(symbol * symbol_list, int symbol_list_length, instruction_data current_instruction, const char ** func_name)
{
    const char * child_func_name;
    *func_name = __func__;

    machine_code_status ret = MACHINE_CODE_STATUS_SUCCESS;

    machine_code instruction_code;
    command curr_command;
    int curr_word_index = 0;
    word_data *curr_word = NULL ;

    operand_content op ;

    instruction_code.word_count = current_instruction.size;
    instruction_code.words = calloc(instruction_code.word_count, sizeof *instruction_code.words);



    g_instructions_word_count += instruction_code.word_count;

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

            // op = &curr_word->content.operand;
            op.funct = curr_command.funct;
            op.dest_operand_type = current_instruction.dest_operand_data.addressing_mode;
            op.dest_register = machine_code_get_operands_register(current_instruction.dest_operand_data);
            op.src_operand_type = current_instruction.src_operand_data.addressing_mode;
            op.src_register = machine_code_get_operands_register(current_instruction.src_operand_data);
            curr_word->content.value = machine_code_reorder_operand_word_content(op);

            curr_word_index++;
            machine_code_status adding_operand;

            if(!flag_array_is_empty(curr_command.src_operand_types, ADDRESSING_TYPES_AMOUNT))
            {
                adding_operand = machine_code_add_operand(symbol_list, symbol_list_length, current_instruction.src_operand_data, &instruction_code, &curr_word_index, &child_func_name);
                if(adding_operand != MACHINE_CODE_STATUS_SUCCESS)
                {
                    machine_code_func_handler(adding_operand,child_func_name);
                }
            }
            if(!flag_array_is_empty(curr_command.dest_operand_types, ADDRESSING_TYPES_AMOUNT))
            {
                adding_operand = machine_code_add_operand(symbol_list, symbol_list_length, current_instruction.dest_operand_data, &instruction_code, &curr_word_index, &child_func_name);
                if(adding_operand != MACHINE_CODE_STATUS_SUCCESS)
                {
                    machine_code_func_handler(adding_operand,child_func_name);
                }
            }
        }

        ret = machine_code_write_machine_code(instruction_code, &child_func_name);

        if (ret != MACHINE_CODE_STATUS_SUCCESS)
        {
            machine_code_func_handler(ret, child_func_name);
        }
        free(instruction_code.words);
    }
    return ret;
}

machine_code_status machine_code_add_symbol_code(symbol current_symbol, const char ** func_name)
{
    *func_name = __func__;
    const char * child_func_name;

    machine_code_status child_func_status;

    machine_code_status ret = MACHINE_CODE_STATUS_SUCCESS;

    /* check if symbol isnt external or label*/
    if(current_symbol.access_attribute != ATTRIBUTE_EXTERN && (current_symbol.size != 0) )
    {
        machine_code symbol_code;
        symbol_code.word_count = current_symbol.size;

        g_symbols_word_count += symbol_code.word_count;

        symbol_code.words = calloc(symbol_code.word_count, sizeof *symbol_code.words);

        if(!symbol_code.words)
        {
            ret = MACHINE_CODE_STATUS_ERROR_MALLOC;

        }
        else
        {
            if(current_symbol.access_attribute == ATTRIBUTE_ENTERY)
            {
                symbol_call entry_item;
                strncpy(entry_item.symbol_name, current_symbol.name, FILE_NAME_LENGTH - 1);
                entry_item.symbol_name[FILE_NAME_LENGTH - 1] = '\0';
                entry_item.base_address = current_symbol.address;

                child_func_status = add_item(entry_item, ATTRIBUTE_ENTERY, &child_func_name);
                if (child_func_status != MACHINE_CODE_STATUS_SUCCESS)
                {
                    machine_code_func_handler(child_func_status, child_func_name);
                }

            }
            for(int word_index = 0; word_index < (int)symbol_code.word_count; word_index++)
            {
                symbol_code.words[word_index].are_attribute =  ABSOLUTE;
                symbol_code.words[word_index].content.value = ((value_content *)current_symbol.data)[word_index];
            }
            child_func_status = machine_code_write_machine_code(symbol_code,&child_func_name);
            if (child_func_status != MACHINE_CODE_STATUS_SUCCESS)
            {
                machine_code_func_handler(child_func_status, child_func_name);
            }
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


machine_code_status machine_code_add_operand(symbol * symbol_list, int symbol_list_length, operand_data operand, machine_code * instruction_code, int * curr_word_index_ptr, const char ** func_name)
{
    *func_name = __func__;
    const char * child_func_name;
    machine_code_status child_func_status;
    machine_code_status ret = MACHINE_CODE_STATUS_SUCCESS;

    int curr_word_index = *curr_word_index_ptr;


    are are_attribute = ABSOLUTE;
    symbol * operand_symbol;

    switch (operand.addressing_mode)
    {
    case ADDRESSING_MODES_IMMEDIATE:
        instruction_code->words[curr_word_index] = (word_data){0};
        instruction_code->words[curr_word_index].are_attribute = ABSOLUTE;

        instruction_code->words[curr_word_index].content.value = operand.operand_data;
        curr_word_index++;
        break;

    case ADDRESSING_MODES_DIRECT:
    case ADDRESSING_MODES_INDEX:
        instruction_code->words[curr_word_index] = (word_data){0};


        if(*operand.varible_name == '\0')
        {

            ret = MACHINE_CODE_STATUS_ERROR_EMPTY_VARIABLE_NAME;
            break;
        }
        operand_symbol = machine_code_find_symbol(symbol_list,symbol_list_length, operand.varible_name);
        if(!operand_symbol)
        {

            ret = MACHINE_CODE_STATUS_ERROR_SYMBOL_NOT_FOUND;
            break;
        }
        if(operand_symbol->access_attribute == ATTRIBUTE_EXTERN)
        {
            are_attribute = EXTERNAL;
            symbol_call extern_item;
            strncpy(extern_item.symbol_name,operand_symbol->name, FILE_NAME_LENGTH - 1);
            extern_item.symbol_name[FILE_NAME_LENGTH - 1] = '\0';
            extern_item.base_address = curr_word_index + g_memory_word_index;
            child_func_status = add_item(extern_item, ATTRIBUTE_EXTERN, &child_func_name);
            if (child_func_status != MACHINE_CODE_STATUS_SUCCESS)
            {
                machine_code_func_handler(child_func_status, child_func_name);
            }
        }
        else
        {
            are_attribute = RELOCATABLE;
        }
        instruction_code->words[curr_word_index].are_attribute = are_attribute;

        instruction_code->words[curr_word_index].content.data_address = (operand_symbol->address / 16) * 16;

        curr_word_index++;

        instruction_code->words[curr_word_index] = (word_data){0};
        instruction_code->words[curr_word_index].are_attribute = are_attribute;
        instruction_code->words[curr_word_index].content.offset = operand_symbol->address % 16;
        curr_word_index++;

        break;

    case ADDRESSING_MODES_REGISTER_DIRECT:
        break;

    default:


        /* check if the operand want just empty, if not error*/
        if((operand.addressing_mode != 0) && (operand.addressing_mode != ADDRESSING_MODES_NONE ) && (operand.operand_data != 0) && (operand.varible_name[0] != '\0'))
        {
            ret = MACHINE_CODE_STATUS_ERROR_UNKNOWN_ADDRESSING_MODE;
        }
        break;
    }

    *curr_word_index_ptr = curr_word_index;
    return ret;
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


void machine_code_func_handler(machine_code_status func_return_status, const char * func_name)
{
    if(func_return_status != MACHINE_CODE_STATUS_SUCCESS)
    {
        g_machine_code_error_flag = true;
    }

    switch (func_return_status)
    {
    case MACHINE_CODE_STATUS_SUCCESS:

        break;

    case MACHINE_CODE_STATUS_ERROR_MALLOC:
        fprintf(stderr, "malloc error in function: %s\n", func_name);
        break;
    case MACHINE_CODE_STATUS_ERROR_REALLOC:
        fprintf(stderr, "realloc error in function: %s\n", func_name);
        break;
    case MACHINE_CODE_STATUS_ERROR_OPENING_FILE:
        fprintf(stderr, "opening file error in function: %s\n", func_name);
        break;
    case MACHINE_CODE_STATUS_ERROR_MEMORY_OVERFLOW:
        fprintf(stderr, "virtual memory overflow error in function: %s\n", func_name);
        break;
    case MACHINE_CODE_STATUS_ERROR_SYMBOL_NOT_FOUND:
        fprintf(stderr, "symbol not found error in function: %s\n", func_name);
        break;
    case MACHINE_CODE_STATUS_ERROR_EMPTY_VARIABLE_NAME:
        fprintf(stderr, "empty variable name error in function: %s\n", func_name);
        break;
    case MACHINE_CODE_STATUS_ERROR_UNKNOWN_ATTRIBUTE_ACCESS_TYPE:
        fprintf(stderr, "uncknown atribute access type error in function: %s\n", func_name);
        break;
    case MACHINE_CODE_STATUS_ERROR_UNKNOWN_ADDRESSING_MODE:
        fprintf(stderr, "uncknown addressing mode error in function: %s\n", func_name);
        break;


    default:
        fprintf(stderr,"unhandled error: %d, in %s\n",func_return_status, func_name);
        break;
    }

}