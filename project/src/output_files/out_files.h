#ifndef OUT_FILES_H
#define OUT_FILES_H

#include "../../include/main.h"
#include "../machine_code/machine_code.h"

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


out_file_status out_files_machine_code(  int machine_code_instructions_length, int machine_code_symbols_length, char* base_file_name, const char ** func_name);

out_file_status out_files_symbol_files(char * base_file_name, out_file_type file_type, const char ** func_name);

void out_files_func_handler(out_file_status func_return_status, const char * func_name);
#endif
