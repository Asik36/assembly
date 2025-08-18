#include "out_files.h"

#define STARTING_GROUP_NAME 'A'
#define OUT_FILE_MSG_ERR_MALLOC                  "malloc error in function: %s\n"
#define OUT_FILE_MSG_ERR_OPENING_FILE            "opening file error in function: %s\n"
#define OUT_FILE_MSG_ERR_UNKNOWN_OUTPUT_FILE_TYPE "unknown output file type error in function: %s\n"
#define OUT_FILE_MSG_ERR_UNHANDLED               "unhandled error: %d, in %s\n"

bool g_out_files_error_flag = false;

bool out_files_main( int machine_code_instructions_length, int machine_code_symbols_length, char* base_file_name)
{
    out_file_status child_func_status = OUT_FILE_STATUS_SUCCESS;
    const char * child_func_name;

    out_files_machine_code(machine_code_instructions_length,  machine_code_symbols_length, base_file_name, &child_func_name);

    child_func_status = out_files_symbol_files(base_file_name, OUT_FILE_TYPE_EXTERNALS, &child_func_name);
    out_files_func_handler(child_func_status, child_func_name);

    child_func_status = out_files_symbol_files(base_file_name, OUT_FILE_TYPE_ENTRIES, &child_func_name);
    out_files_func_handler(child_func_status, child_func_name);

    return g_out_files_error_flag == false;
}

/**
 * @brief a helper function that converts a word from the machine code table to special base (splits the 20 bits into 5 groups of 4)
 *
 * @param word the word_data from memory to be converted to the special base
 * @return special_base the word data after being changed to the special base
 */
static special_base out_files_convert_to_special_base(word_data word)
{
    special_base ret = (special_base){0};

    uint32_t are_bits  = ((uint32_t)word.are_attribute) & 0xF; /* make sure to keep only 4 bits*/
    uint32_t val_bits = ((uint32_t)word.content.value) & 0xFFFF; /* make sure to keep only 16 bits*/
    uint32_t packed = (are_bits << 16) | val_bits; /* unite are bits and value bits*/



    for (int group_index = 0; group_index < AMOUNT_OF_SPETIAL_BITS_GROUPS; group_index++)
    {
        int shift = (AMOUNT_OF_SPETIAL_BITS_GROUPS - 1 - group_index) * SIZE_OF_SPETIAL_BITS_GROUP;
        ret.groups[group_index].val = (uint16_t)((packed >> shift) & 0xF);
    }
    return ret;
}

/**
 * @brief a helper function that writes one word from the memory into the machine code output file in the correct format
 *
 * @param machine_code_f a pointer to the machine code output file
 * @param word_index the index in the g_memory of the word we want to write into the output file
 */
static void out_files_write_machine_code_line(FILE * machine_code_f, int word_index)
{
    special_base line = out_files_convert_to_special_base(g_memory[word_index]);
    fprintf(machine_code_f,  "%04d ", word_index);

    char ending_char = '-';

    for(int group_index = 0; group_index < AMOUNT_OF_SPETIAL_BITS_GROUPS; group_index++)
    {
        /*check if this is not the last group, and set the char after the groups value acordingly*/
        if(group_index + 1 < AMOUNT_OF_SPETIAL_BITS_GROUPS)
        {
            ending_char = '-';
        }
        else
        {
            ending_char = '\n';
        }

        fprintf(machine_code_f, "%c%x%c", STARTING_GROUP_NAME + group_index,line.groups[group_index].val, ending_char);
    }
}

/**
 * @brief a helperr function that creates an output file with appropriate name 
 *
 * @param f_type the type of output file to be created
 * @param base_file_name the name of the base file (the name of the assembly file without the ending)
 * @param out_file a pointer to a pointer to a file that is used to pass up to the called function the opend file
 * @param func_name a string that is used to return this functions name to the caller function
 * @return out_file_status status that represents wether the function was executed successfully or with wich errors
 */
static out_file_status out_files_create_file(out_file_type f_type, char * base_file_name,FILE ** out_file, const char ** func_name)
{
    char ret_file_name [FILE_NAME_LENGTH];
    char * file_ending = NULL;
    out_file_status ret = OUT_FILE_STATUS_SUCCESS;
    *func_name = __func__;

    switch (f_type)
    {
    case OUT_FILE_TYPE_OBJECT:
        file_ending = "ob";
        break;
    case OUT_FILE_TYPE_EXTERNALS:
        file_ending = "ext";
        break;
    case OUT_FILE_TYPE_ENTRIES:
        file_ending = "ent";
        break;

    default:
        ret = OUT_FILE_STATUS_ERROR_UNKNOWN_OUTPUT_FILE_TYPE;
        break;
    }
    if(file_ending)
    {
        snprintf(ret_file_name , FILE_NAME_LENGTH,"%s.%s", base_file_name,file_ending);
        *out_file = fopen(ret_file_name,"w");
        if (!out_file)
        {
            ret = OUT_FILE_STATUS_ERROR_OPENING_FILE;
        }
    }
    return ret;
}

void out_files_machine_code(  int machine_code_instructions_length, int machine_code_symbols_length, char* base_file_name, const char ** func_name)
{
    *func_name = __func__;

    const char * child_func_name;
    out_file_status child_func_status;

    FILE * machine_code_f = NULL;
    child_func_status = out_files_create_file(OUT_FILE_TYPE_OBJECT, base_file_name, &machine_code_f, &child_func_name);
    out_files_func_handler(child_func_status,child_func_name);

    if(child_func_status == OUT_FILE_STATUS_SUCCESS)
    {
        /*file header: the length of instructions in words then the length of symbols in words (in the memory)*/
        fprintf(machine_code_f, "%d\t%d\n", machine_code_instructions_length, machine_code_symbols_length);

        /*file content: each line has 4 digits with the number of word in the memorry, then the name of the special group of 4 bits, and the 4 bits in hexadecimal representation, seperated by '-'*/
        for(int word_index = STARTING_MEMORY_ADDRESS; word_index < STARTING_MEMORY_ADDRESS + machine_code_instructions_length + machine_code_symbols_length; word_index++)
        {
            out_files_write_machine_code_line(machine_code_f, word_index);
        }

        fclose(machine_code_f);
    }
}

out_file_status out_files_write_line(FILE * out_file, out_file_type file_type, int symbol_index, const char ** func_name)
{
    out_file_status ret = OUT_FILE_STATUS_SUCCESS;
    *func_name = __func__;

    symbol_call symbol_to_write;
    switch (file_type)
    {
    case OUT_FILE_TYPE_ENTRIES:
        symbol_to_write = g_entrys[symbol_index];
        fprintf(out_file, "%s,%d,%d\n",symbol_to_write.symbol_name, (symbol_to_write.base_address & MASK_BASE_ADDRESS), (symbol_to_write.base_address & MASK_OFFSET_ADDRESS));
        break;
    case OUT_FILE_TYPE_EXTERNALS:
        symbol_to_write = g_externals[symbol_index];
        fprintf(out_file, "%s BASE %d\n", symbol_to_write.symbol_name, symbol_to_write.base_address);
        fprintf(out_file, "%s OFFSET %d\n\n", symbol_to_write.symbol_name, symbol_to_write.base_address + 1);
        break;
    default:
        ret = OUT_FILE_STATUS_ERROR_UNKNOWN_OUTPUT_FILE_TYPE;
        break;
    }

    return ret;
}

out_file_status out_files_symbol_files(char * base_file_name, out_file_type file_type, const char ** func_name)
{
    out_file_status ret = OUT_FILE_STATUS_SUCCESS;
    *func_name = __func__;

    out_file_status child_func_status;
    const char * child_func_name;

    FILE * out_file;

    int length = 0;

    switch (file_type)
    {
    case OUT_FILE_TYPE_ENTRIES:
        length = g_entry_defenition_length;
        break;
    case OUT_FILE_TYPE_EXTERNALS:
        length = g_extern_call_length;
        break;
    default:
        ret = OUT_FILE_STATUS_ERROR_UNKNOWN_OUTPUT_FILE_TYPE;
        break;
    }

    if(length)
    {
        child_func_status = out_files_create_file(file_type, base_file_name, &out_file,&child_func_name);
        out_files_func_handler(child_func_status,child_func_name);

        if(child_func_status == OUT_FILE_STATUS_SUCCESS)
        {
            for(int call_index = 0; call_index < length; call_index++)
            {
                out_files_write_line(out_file, file_type, call_index, &child_func_name);
                out_files_func_handler(child_func_status,child_func_name);
            }
            fclose(out_file);
        }
    }
    return ret;
}

void out_files_func_handler(out_file_status func_return_status, const char * func_name)
{
    if(func_return_status != OUT_FILE_STATUS_SUCCESS)
    {
        g_out_files_error_flag = true;
    }

    switch (func_return_status)
    {
    case OUT_FILE_STATUS_SUCCESS:
        break;

    case OUT_FILE_STATUS_ERROR_MALLOC:
        fprintf(stderr, OUT_FILE_MSG_ERR_MALLOC, func_name);
        break;

    case OUT_FILE_STATUS_ERROR_OPENING_FILE:
        fprintf(stderr, OUT_FILE_MSG_ERR_OPENING_FILE, func_name);
        break;

    case OUT_FILE_STATUS_ERROR_UNKNOWN_OUTPUT_FILE_TYPE:
        fprintf(stderr, OUT_FILE_MSG_ERR_UNKNOWN_OUTPUT_FILE_TYPE, func_name);
        break;

    default:
        fprintf(stderr, OUT_FILE_MSG_ERR_UNHANDLED, func_return_status, func_name);
        break;
    }

}