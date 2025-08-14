#include "out_files.h"

#define STARTING_GROUP_NAME 'A'


void out_files_main( int machine_code_instructions_length, int machine_code_symbols_length, char* base_file_name)
{
    out_files_machine_code(  machine_code_instructions_length,  machine_code_symbols_length, base_file_name);
    out_files_symbol_files(base_file_name, OUT_FILE_TYPE_EXTERNALS);
    out_files_symbol_files(base_file_name, OUT_FILE_TYPE_ENTRIES);
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

static bool out_files_create_file(out_file_type f_type, char * base_file_name,FILE ** out_file)
{
    char ret_file_name [FILE_NAME_LENGTH];
    char * file_ending = NULL;
    bool ret = true;

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
        fprintf(stderr, "%s error: unknown output file type: %d\n",__func__, f_type);
        ret = false;
        break;
    }
    if(file_ending)
    {
        snprintf(ret_file_name , FILE_NAME_LENGTH,"%s.%s", base_file_name,file_ending);
        *out_file = fopen(ret_file_name,"w");
    }
    return(ret);
}

void out_files_machine_code(  int machine_code_instructions_length, int machine_code_symbols_length, char* base_file_name)
{

    FILE * machine_code_f ;
    if(out_files_create_file(OUT_FILE_TYPE_OBJECT, base_file_name, &machine_code_f))
    {
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
}

void out_files_write_line(FILE * out_file, out_file_type file_type, int symbol_index)
{
    symbol_call symbol_to_write;
    switch (file_type)
    {
    case OUT_FILE_TYPE_ENTRIES:
        symbol_to_write = g_entrys[symbol_index];
        fprintf(out_file, "%s,%d,%d\n",symbol_to_write.symbol_name, (symbol_to_write.base_address / 16) * 16, symbol_to_write.base_address % 16);
        break;
    case OUT_FILE_TYPE_EXTERNALS:
        symbol_to_write = g_externals[symbol_index];
        fprintf(out_file, "%s BASE %d\n", symbol_to_write.symbol_name, symbol_to_write.base_address);
        fprintf(out_file, "%s OFFSET %d\n\n", symbol_to_write.symbol_name, symbol_to_write.base_address + 1);
        break;
    default:
        fprintf(stderr, "%s error: not a supported out file type: %d\n",__func__, file_type);
        break;
    }


}

void out_files_symbol_files(char * base_file_name, out_file_type file_type)
{
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
        fprintf(stderr, "%s error: unknown output file type: %d\n",__func__, file_type);
        break;
    }

    if(length)
    {
        FILE * out_file;
        if( out_files_create_file(file_type, base_file_name, &out_file))
        {
            if(!out_file)
            {
                char * file_type_name;
                if(file_type == OUT_FILE_TYPE_EXTERNALS)
                {
                    file_type_name = "externals";
                }
                else
                {
                    file_type_name = "entries";
                }

                fprintf(stderr,"%s error: opening %s file\n", __func__, file_type_name);
            }
            else
            {
                for(int call_index = 0; call_index < length; call_index++)
                {
                    out_files_write_line(out_file, file_type, call_index);
                }
                fclose(out_file);
            }
        }
    }

}

