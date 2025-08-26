#ifndef MACHINE_CODE_H
#define MACHINE_CODE_H
#include <stdio.h>
#include "common_structs.h"
#include <stdlib.h>
#include <string.h>

#define MEMORY_WORD_LENGTH  20
#define STARTING_MEMORY_ADDRESS 100
#define FILE_NAME_LENGTH 255

/*masks used to create base and offset addreses from the regular (value) address*/
#define MASK_OFFSET_ADDRESS 0x000F
#define MASK_BASE_ADDRESS 0xFFF0

extern word_data g_memory[MEMORY_MAX_SIZE];


extern const char * function_names [];
typedef struct machine_code_symbol_call_s
{
    char symbol_name[FILE_NAME_LENGTH];
    int base_address;
}symbol_call;


extern symbol_call * g_externals;
extern symbol_call * g_entrys;

extern int g_extern_call_length ;
extern int g_entry_defenition_length ;

typedef enum machine_code_success_e
{
    MACHINE_CODE_STATUS_ERROR_MALLOC = -9 ,
    MACHINE_CODE_STATUS_ERROR_REALLOC,
    MACHINE_CODE_STATUS_ERROR_SYMBOL_NOT_FOUND,
    MACHINE_CODE_STATUS_ERROR_OPENING_FILE,
    MACHINE_CODE_STATUS_ERROR_UNKNOWN_ATTRIBUTE_ACCESS_TYPE,
    MACHINE_CODE_STATUS_ERROR_UNKNOWN_ADDRESSING_MODE,
    MACHINE_CODE_STATUS_ERROR_EMPTY_VARIABLE_NAME,
    MACHINE_CODE_STATUS_ERROR_MISSING_OPERAND,
    MACHINE_CODE_STATUS_ERROR_MEMORY_OVERFLOW,

    MACHINE_CODE_STATUS_SUCCESS = 0,



}machine_code_status;



/**
 * @brief the function creates the machine code memory aarray and calls fnctions that fill in the instructions and declarations into it
 *
 * @param symbol_list an array of all the symbols
 * @param symbol_list_length the amount of symbols in the symbol array
 * @param instruction_list an array of all the instructions
 * @param instruction_list_length the amount of instructions in the symbol array
 */
bool machine_code_main(char * base_file_name,symbol_t * symbol_list, int symbol_list_length, instruction_data_t * instruction_list, int instruction_list_length);

/**
 * @brief the function tries to write machine code into the memory array
 *
 * @param code the machine code to add to the memory
 * @param func_name a string that is used to return this functions name to the caller function
 * @return machine_code_status status_e that represents wether the function was executed successfully or with wich errors
 * otherwise adds machine code to memory and return SUCCESS
 */
machine_code_status machine_code_write_machine_code(machine_code code, const char ** func_name);

/**
 * @brief the function adds instructions to memory one by one, until there are no more instructions left or the addition of one of the instructions is failed
 *
 * @param symbol_list an array of all the symbols
 * @param symbol_list_lengththe amount of symbols in the symbol array
 * @param instruction_list an array of all the instructions
 * @param instruction_list_length the amount of instructions in the symbol array
 */
void machine_code_handle_instructions(symbol_t * symbol_list, int symbol_list_length, instruction_data_t * instruction_list, int instruction_list_length);

/**
 * @brief the function adds instructions to memory one by one, until there are no more instructions left or the addition of one of the symbols is failed
 *
 * @param symbol_list  an array of all the symbols
 * @param symbol_list_lengththe amount of symbols in the symbol array
 */
void machine_code_handle_symbols(symbol_t * symbol_list, int symbol_list_length);

/**
 * @brief the function adds one instruction to the memory array
 *
 * @param symbol_list an array of all the symbols
 * @param symbol_list_lengththe amount of symbols in the symbol array
 * @param current_instruction the instruction to be added to the memory
 * @param func_name a string that is used to return this functions name to the caller function
 * @return machine_code_status status_e that represents wether the function was executed successfully or with wich errors
 */
machine_code_status machine_code_add_instruction_code(symbol_t * symbol_list, int symbol_list_length, instruction_data_t current_instruction, const char ** func_name);

/**
 * @brief the function adds one symbol to the memory array
 *
 * @param current_symbol the symbol_t to be added to the memory
 * @param func_name a string that is used to return this functions name to the caller function
 * @return machine_code_status status_e that represents wether the function was executed successfully or with wich errors
 */
machine_code_status machine_code_add_symbol_code(symbol_t current_symbol, const char ** func_name);

/**
 * @brief the function saerches for a symbol by name in the symbol table
 *
 * @param symbol_list an array of all the symbols
 * @param symbol_list_lengththe amount of symbols in the symbol_t array
 * @param symbol_name the name that is searched for in the symbol_t array
 * @return symbol_t* a pointer to the symbol with the name field identical to symbol_name, or  NULL if such a symbol wasn't found
 */
symbol_t* machine_code_find_symbol(symbol_t * symbol_list, int symbol_list_length, const char * symbol_name);

/**
 * @brief the function adds to the machine additional words if needed for an operand
 *
 * @param symbol_list an array of all the symbols
 * @param symbol_list_length the amount of symbols in the symbol array
 * @param operand the operand that may need extra words added
 * @param instruction_code the machine code pointer wich the extra words are added to
 * @param curr_word_index the current word to write to in the words array (field in instruction_code)
 * @param func_name a string that is used to return this functions name to the caller function
 * @return int the current word to write to in the words array at the end of adding all needed additional words
 */
machine_code_status machine_code_add_operand(symbol_t * symbol_list, int symbol_list_length, operand_data_t operand, machine_code * instruction_code, int * curr_word_index_ptr, const char ** func_name);

/**
 * @brief the function gets an operand and returns what needs to be stored in it's corresponding register
 *
 * @param operand the operand that we want to check its mode and register
 * @return uint16_t if the operands mode uses a register return the register(the operand data), otherwise retrun 0
 */
uint16_t machine_code_get_operands_register(operand_data_t operand);

/**
 * @brief the function prints an error message, that includes the type of error and the function it happend in
 *
 * @param func_return_status the status_e value of the function
 * @param func_name  a string with the name of the function that was called
 */
void machine_code_func_handler(machine_code_status func_return_status, const char * func_name);

/**
 * @brief Add second instruction word to machine code
 * 
 * @param instruction_code the machine code pointer wich the extra words are added to
 * @param current_instruction the instruction to be added to the memory
 * @param symbol_list an array of all the symbols
 * @param symbol_list_length the amount of symbols in the symbol array
 * @param func_name a string with the name of the function that was called
 */
void machine_code_instructions_with_operand_word(machine_code * instruction_code, instruction_data_t current_instruction,symbol_t *symbol_list, int symbol_list_length,const char **func_name);

#endif