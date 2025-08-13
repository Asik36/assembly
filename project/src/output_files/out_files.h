#include "main.h"
#include "machine_code.h"

#define SIZE_OF_SPETIAL_BITS_GROUP 4
#define AMOUNT_OF_SPETIAL_BITS_GROUPS 5

typedef struct special_bits_group_s
{
    uint8_t val : SIZE_OF_SPETIAL_BITS_GROUP;
}special_bits_group;

typedef struct special_base_s
{
    special_bits_group groups [AMOUNT_OF_SPETIAL_BITS_GROUPS];
}special_base;

/**
 * @brief the function creates all the nessessery output files (machine code file, extern file, entry file)
 *
 * @param machine_code_table an array of machine code (used to create machine code file, and extern file)
 * @param machine_code_length the amount of machine code that was writen
 * @param symbol_list an array of all the symbols (used to create the entry file)
 * @param symbol_list_length the amount of symbols in the symbol array
 */
void out_files_main(machine_code * machine_code_table, int machine_code_length, symbol * symbol_list, int symbol_list_length);

/**
 * @brief the function creates the machine code file and fills it with the machine code in the speacitial base format
 *
 * @param machine_code_table an array of machine code
 * @param machine_code_length the amount of machine code that was writen
 */
void out_files_machine_code(machine_code * machine_code_table, int machine_code_length);

/**
 * @brief the function creates the extern file and fills it with all the
 *
 * @param machine_code_table an array of machine code
 * @param machine_code_length the amount of machine code that was writen
 */
void out_files_extern(machine_code * machine_code_table, int machine_code_length);

/**
 * @brief the function creates the entry file and fills it with all the
 *
 * @param symbol_list an array of all the symbols
 * @param symbol_list_length the amount of symbols in the symbol array
 */
void out_files_entry( symbol * symbol_list, int symbol_list_length);