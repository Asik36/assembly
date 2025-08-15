#ifndef OUT_FILES_H
#define OUT_FILES_H

#include "common_structs.h"
#include "machine_code.h"

#define SIZE_OF_SPETIAL_BITS_GROUP 4
#define AMOUNT_OF_SPETIAL_BITS_GROUPS 5


typedef struct special_bits_group_s
{
    uint16_t val : SIZE_OF_SPETIAL_BITS_GROUP;
} special_bits_group;

typedef struct special_base_s
{
    special_bits_group groups[AMOUNT_OF_SPETIAL_BITS_GROUPS];
} special_base;

typedef enum out_filetype_e
{
    OUT_FILE_TYPE_OBJECT,
    OUT_FILE_TYPE_EXTERNALS,
    OUT_FILE_TYPE_ENTRIES
}out_file_type;

typedef enum out_file_status_e
{
    OUT_FILE_STATUS_SUCCESS,
    OUT_FILE_STATUS_ERROR_MALLOC,
    OUT_FILE_STATUS_ERROR_OPENING_FILE,
    OUT_FILE_STATUS_ERROR_UNKNOWN_OUTPUT_FILE_TYPE,
}out_file_status;

bool out_files_main( int machine_code_instructions_length, int machine_code_symbols_length, char* base_file_name);

/**
 * @brief the function creates the machine code file and fills it with the machine code from the global memory array
 *
 * @param machine_code_instructions_length the length in words of the instructions in the memory array
 * @param machine_code_symbols_length the  length in words of the symbols in the memory array
 * @param base_file_name the name of the base file (the name of the assembly file without the ending)
 * @param func_name a string that is used to return this functions name to the caller function
 * @return out_file_status status that represents wether the function was executed successfully or with wich errors
 */
out_file_status out_files_machine_code(  int machine_code_instructions_length, int machine_code_symbols_length, char* base_file_name, const char ** func_name);

/**
 * @brief the function writes a single symbol call into either the entry or the external output files
 *
 * @param out_file a file pointer to the output file to write the line to
 * @param file_type the type of file to write to (extern or entry) that determins the format of the line
 * @param symbol_index the index of the symbol call to be writen (in its apropriate global array )
 * @param func_name a string that is used to return this functions name to the caller function
 * @return out_file_status status that represents wether the function was executed successfully or with wich errors
 */
out_file_status out_files_write_line(FILE * out_file, out_file_type file_type, int symbol_index, const char ** func_name);

/**
 * @briefthe function writes a hole symbol call array into its appropriate output file
 *
 * @param base_file_name the name of the base file (the name of the assembly file without the ending)
 * @param file_type the type of file to write to (extern or entry) that determins the global array that the symbol calls are red from
 * @param func_name a string that is used to return this functions name to the caller function
 * @return out_file_status status that represents wether the function was executed successfully or with wich errors
 */
out_file_status out_files_symbol_files(char * base_file_name, out_file_type file_type, const char ** func_name);

/**
 * @brief the function prints an error message, that includes the type of error and the function it happend in
 *
 * @param func_return_status the status value of the function
 * @param func_name a string with the name of the function that was called
 */
void out_files_func_handler(out_file_status func_return_status, const char * func_name);
#endif
