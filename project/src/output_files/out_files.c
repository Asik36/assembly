#include "out_files.h"

void out_files_main( int machine_code_instructions_length, int machine_code_symbols_length, symbol * symbol_list, int symbol_list_length, char* base_file_name)
{
    out_files_machine_code(  machine_code_instructions_length,  machine_code_symbols_length, base_file_name);
    out_files_extern(  machine_code_instructions_length,  machine_code_symbols_length);
    out_files_entry(symbol_list, symbol_list_length);
}


void out_files_machine_code(  int machine_code_instructions_length, int machine_code_symbols_length, char* base_file_name)
{
    char * machine_code_file_name;
    sprintf(machine_code_file_name ,"%s.object", base_file_name);
    FILE * machine_code_f = fopen(machine_code_file_name, "w");
    if(! machine_code_f)
    {
        exit(1);
        /*add  error handle*/
    }

    else
    {
        fprintf(machine_code_f, "%d\t%d\n", machine_code_instructions_length, machine_code_symbols_length);

        special_base line;
        for(int word_index = STARTING_MEMORY_ADDRESS; word_index < machine_code_instructions_length + machine_code_symbols_length; word_index++)
        {
            line = (special_base *) g_memory[word_index];
        }
    }
}
void out_files_extern(int machine_code_instructions_length, int machine_code_symbols_length)
{
    /*code*/
}
void out_files_entry( symbol * symbol_list, int symbol_list_length)
{
    /*CODE*/
}