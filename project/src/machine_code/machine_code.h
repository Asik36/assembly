#ifndef MACHINE_CODE_H
#define MACHINE_CODE_H
#include <stdio.h>
#include "../../include/main.h"
#include <stdlib.h>
#include <string.h>

#define MEMORY_WORD_LENGTH  20
#define STARTING_MEMORY_ADDRESS 100
#define FILE_NAME_LENGTH 255

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
    MACHINE_CODE_STATUS_SUCCESS ,
    MACHINE_CODE_STATUS_ERROR_MALLOC,
    MACHINE_CODE_STATUS_ERROR_SYMBOL_NOT_FOUND,

}machine_code_status;

typedef enum machine_code_func_type_e
{
    FUNC_TYPE_MAIN,
    FUNC_TYPE_WRITE_MACHINE_CODE,
    FUNC_TYPE_HANDLE_INSTRUCTIONS,
    FUNC_TYPE_HANDLE_SYMBOLS,
    FUNC_TYPE_ADD_INSTRUCTION_CODE,
    FUNC_TYPE_ADD_SYMBOL_CODE,
    FUNC_TYPE_FIND_SYMBOL,
    FUNC_TYPE_ADD_OPERAND,
    FUNC_TYPE_GET_OPERANDS_REGISTER,
    FUNC_TYPE_FUNC_HANDLER,
} func_type;

/**
 * @brief the function creates the machine code memory aarray and calls fnctions that fill in the instructions and declarations into it
 *
 * @param symbol_list an array of all the symbols
 * @param symbol_list_length the amount of symbols in the symbol array
 * @param instruction_list an array of all the instructions
 * @param instruction_list_length the amount of instructions in the symbol array
 */
bool machine_code_main(char * base_file_name,symbol * symbol_list, int symbol_list_length, instruction_data * instruction_list, int instruction_list_length);

/**
 * @brief the function tryies to write machine code into the memory array
 *
 * @param code the machine code to add to the memory
 * @return machine_code_status FAILURE if there isn't enough space for the machine code (and doesnt add enything to the memory),
 * otherwise adds machine code to memory and return SUCCESS
 */
machine_code_status machine_code_write_machine_code(machine_code code);

/**
 * @brief the function adds instructions to memory one by one, until there are no more instructions left or the addition of one of the instructions is failed
 *
 * @param symbol_list an array of all the symbols
 * @param symbol_list_lengththe amount of symbols in the symbol array
 * @param instruction_list an array of all the instructions
 * @param instruction_list_length the amount of instructions in the symbol array
 */
void machine_code_handle_instructions(symbol * symbol_list, int symbol_list_length, instruction_data * instruction_list, int instruction_list_length);

/**
 * @brief the function adds instructions to memory one by one, until there are no more instructions left or the addition of one of the symbols is failed
 *
 * @param symbol_list  an array of all the symbols
 * @param symbol_list_lengththe amount of symbols in the symbol array
 */
void machine_code_handle_symbols(symbol * symbol_list, int symbol_list_length);

/**
 * @brief the function adds one instruction to the memory array
 *
 * @param symbol_list an array of all the symbols
 * @param symbol_list_lengththe amount of symbols in the symbol array
 * @param current_instruction the instruction to be added to the memory
 * @return machine_code_status SUCCESS if the instruction was added to the memory, otherwise FAILURE
 */
machine_code_status machine_code_add_instruction_code(symbol * symbol_list, int symbol_list_length, instruction_data current_instruction);

/**
 * @brief the function adds one symbol to the memory array
 *
 * @param current_symbol the symbol to be added to the memory
 * @return machine_code_status SUCCESS if the instruction was added to the memory, otherwise FAILURE
 */
machine_code_status machine_code_add_symbol_code(symbol current_symbol);

/**
 * @brief the function saerches for a symbol by name in the symbol table
 *
 * @param symbol_list an array of all the symbols
 * @param symbol_list_lengththe amount of symbols in the symbol array
 * @param symbol_name the name that is searched for in the symbol array
 * @return symbol* a pointer to the symbol with the name field identical to symbol_name, or  NULL if such a symbol wasn't found
 */
symbol* machine_code_find_symbol(symbol * symbol_list, int symbol_list_length, const char * symbol_name);

/**
 * @brief the function adds to the machine additional words if needed for an operand
 *
 * @param symbol_list an array of all the symbols
 * @param symbol_list_lengththe amount of symbols in the symbol array
 * @param operand the operand that may need extra words added
 * @param instruction_code the machine code pointer wich the extra words are added to
 * @param curr_word_index the current word to write to in the words array (field in instruction_code)
 * @return int the current word to write to in the words array at the end of adding all needed additional words
 */
int machine_code_add_operand(symbol * symbol_list, int symbol_list_length, operand_data operand, machine_code * instruction_code, int curr_word_index);

/**
 * @brief the function gets an operand and returns what needs to be stored in it's corresponding register
 *
 * @param operand the operand that we want to check its mode and register
 * @return uint16_t if the operands mode uses a register return the register(the operand data), otherwise retrun 0
 */
uint16_t machine_code_get_operands_register(operand_data operand);

/**
 * @brief the function prints an error message, that includes the type of error and the function it happend in
 *
 * @param ret the status value of the function
 * @param func the function that returned the status
 */
void machine_code_func_handler(machine_code_status ret, func_type func);

#endif