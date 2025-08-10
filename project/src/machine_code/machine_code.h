#ifndef MACHINE_CODE_H
#define MACHINE_CODE_H
#include <stdio.h>
#include "main.h"
#include <stdlib.h>
#include <string.h>

#define MEMORY_WORD_LENGTH  20
#define STARTING_MEMORY_ADDRESS 100
/**
 * @brief the function creates the machine code memory aarray and calls fnctions that fill in the instructions and declarations into it
 *
 * @param symbol_list an array of all the symbols
 * @param symbol_list_length the amount of symbols in the symbol array
 * @param instruction_list an array of all the instructions
 * @param instruction_list_length the amount of instructions in the symbol array
 */
void machine_code_main(symbol * symbol_list, int symbol_list_length, instruction * instruction_list, int instruction_list_length);

/**
 * @brief the function tryies to write machine code into the memory array
 *
 * @param code the machine code to add to the memory
 * @return status FAILURE if there isn't enough space for the machine code (and doesnt add enything to the memory),
 * otherwise adds machine code to memory and return SUCCESS
 */
status machine_code_write_machine_code(machine_code code);

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
 * @brief the function adds one instruction to the memory array
 *
 * @param symbol_list an array of all the symbols
 * @param symbol_list_lengththe amount of symbols in the symbol array
 * @param current_instruction the instruction to be added to the memory
 * @return status SUCCESS if the instruction was added to the memory, otherwise FAILURE
 */
status machine_code_add_instruction_code(symbol * symbol_list, int symbol_list_length, instruction_data current_instruction);

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

#endif