#include "out_files.h"

#define STARTING_GROUP_NAME 'A'

void out_files_main( int machine_code_instructions_length, int machine_code_symbols_length, symbol * symbol_list, int symbol_list_length, char* base_file_name)
{
    out_files_machine_code(  machine_code_instructions_length,  machine_code_symbols_length, base_file_name);
    // out_files_extern(  machine_code_instructions_length,  machine_code_symbols_length);
    // out_files_entry(symbol_list, symbol_list_length);
}

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

static void out_files_write_machine_code_line(FILE * machine_code_f, int word_index)
{
    special_base line = out_files_convert_to_special_base(g_memory[word_index]);
    fprintf(machine_code_f,  "%04d ", word_index);

    char ending_char;

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

void out_files_machine_code(  int machine_code_instructions_length, int machine_code_symbols_length, char* base_file_name)
{
    char machine_code_file_name [FILE_NAME_LENGTH];
    snprintf(machine_code_file_name , FILE_NAME_LENGTH,"%s.object", base_file_name);
    FILE * machine_code_f = fopen(machine_code_file_name, "w");
    if(! machine_code_f)
    {
        fprintf(stderr,"error opening file: %s, in: %s\n",base_file_name, __func__);
        exit(1);
        /*add  error handle*/
    }

    else
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
// void out_files_extern(int machine_code_instructions_length, int machine_code_symbols_length)
// {
//     /*code*/
// }
// void out_files_entry( symbol * symbol_list, int symbol_list_length)
// {
//     /*CODE*/
// }