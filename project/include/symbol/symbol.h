#ifndef SYMBOL_H
#define SYMBOL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include "common_structs.h"
#define START_ADDRESS 100
#define MSG_SUCCESS                         ""
#define MSG_ERR_EXTERN_DEFINED_IN_FILE      "ERROR : Extern can't be defined in a file.\n"
#define MSG_ERR_ALREADY_EXTERN              "ERROR : A entery symbol is already defined as extern.\n"
#define MSG_ERR_DATA_ALREADY_DEFINED        "ERROR : Multiple labels with the same name\n"
#define MSG_ERR_UNINITILIZED_ENTRY          "ERROR : Entry is uninitialized\n"
#define MSG_ERR_MEMORY_ALLOCATION           "ERROR : Memory allocation failed\n"
#define MSG_ERR_MEMORY_OVERFLOW             "ERROR : Address memory out of bounds\n"
#define MSG_ERR_UNDEFINED                   "ERROR : Undefined\n"
typedef enum 
{
    STATUS_ERR_MEMORY_OVERFLOW = -6,
    STATUS_ERR_MEMORY_ALLOCATION,
    STATUS_ERR_UNINITILIZED_ENTRY,
    STATUS_ERR_EXTERN_DEFINED_IN_FILE,
    STATUS_ERR_ALREADY_EXTERN,
    STATUS_ERR_DATA_ALREADY_DEFINED,
    STATUS_SUCCESS = 0,
    STATUS_NO_LABEL,
    STATUS_SYMBOL_UPDATE,
    STATUS_SYMBOL_ENTER


} status_e;


/**
 * @brief Creates symbol table from instructions and directive
 * @param instruction_data_table instruction data list
 * @param instruction_table instruction strings
 * @param instruction_num amont of instruction
 * @param directive_table list of directive sentence
 * @param directive_num amont of drrective sentences
 * @param p_symbol_table pointer to list of symbols
 * @param p_symbol_counter pointer to number of symbols
 * @return return status that tells if symbol table created succssfully or not
 */
status_e symbol_create(instruction_data *instruction_data_table, instruction *instruction_table, int instruction_num,
                       directive *directive_table, int directive_num,
                       symbol **p_symbol_table, int *p_symbol_counter);


/**
 * 
 * @brief fill symbols from instruction sentences
 * @param instruction_data_table instruction data list
 * @param instruction_table instruction strings
 * @param instruction_num amont of instruction
 * @param p_symbol_table pointer to list of symbols
 * @param p_symbol_counter pointer to number of symbols
 * @return status type of error on symbol creation or succsess
 */
status_e symbol_instructions(   instruction_data * instruction_data_table, instruction * instruction_table, int instruction_num,
                                symbol           ** p_symbol_table          , int * p_symbol_counter);
/**
 * @brief fill symbols from directive sentences
 * @param directive_table list of directive sentence
 * @param directive_num amont of drrective sentences
 * @param p_symbol_table pointer to list of symbols
 * @param p_symbol_counter pointer to number of symbols
 * @return status type of error on symbol creation or succsess
 */
status_e symbol_directive(directive *directive_table, int directive_num,
                          symbol **p_symbol_table, int *p_symbol_counter);


/**
 * @brief fill the remaining unsigned address of symbols and checks for errors in the symbol table
 * @param p_symbol_table pointer to list of symbols
 * @param symbol_counter amont of symbols
 * @param start_address defualt start adress in that unisgined address and will start from
 * @return status type of error on existing symbol or succsess if no errors
 */
status_e symbol_complete_table(symbol * p_symbol_table, int symbol_counter, int start_address);


/**
 * @brief get label from instruction and add it to list if it does not exists. includes the address of the current instruction
 * @param ins current instruction
 * @param ins_data current instruction data
 * @param symbol_table list of symbols
 * @param index offset of the current symbol
 * @return return error if label alredy exists otherwise return succsess
 */
status_e symbol_get_instruction_labels(instruction *ins, instruction_data *ins_data, symbol *symbol_table,int index);

/**
 * @brief get label from directive and add it to list if. updatess data atterbiute or accsess attribute if alredy exists
 * @param dir current directive
 * @param symbol_table list of symbols
 * @param index offset of the current symbol
 * @return return code error if label alredy exists or is unvaild, otherwise return succsess
 */
status_e symbol_get_directive_labels(directive *dir, symbol *symbol_table,int index);

/**
 * @brief copys data from directive into symbol
 * @param new_symbol dest symbol
 * @param dir source directory
 */
void symbol_extract_directive(symbol *new_symbol, directive *dir);
/**
 * @brief search for symbol index inside a hashmap using symbol name
 * @param symbol_name key name for searching in hashmap
 * @return pointer to index of symbol table
 */
int * symbol_find(char *symbol_name);
/**
 * @brief enter a new index of symbol table into a hashmap
 * @param symbol_name key name for searching in hashmap
 * @return if symbol name does not exists returns its index, else returns -1
 */
int symbol_enter(char *symbol_name,int index);

/**
 * @brief copies the data from new_symbol to old symbol and checks for errors
 * @param old_symbol dest symbol
 * @param new_symbol src symbol
 * @return succsess or type of error
 */
status_e symbol_update(symbol *old_symbol, symbol *new_symbol) ;

/**
 * @brief checks for errors in case symbol has attribute extern
 * @param old_symbol symbol with attributre extern
 * @return error if extern is alredy defined inside the fille 
 */
status_e symbol_handle_extern_attribute(symbol *old_symbol);
/**
 * @brief checks for errors in case symbol has entery extern
 * @param old_symbol symbol with attributre extern
 * @return error if symbol alredy defined as extern 
 */
status_e symbol_handle_entry_attribute(symbol *old_symbol) ;
/**
 * @brief checks for errors in case symbol has entery data or string
 * @param old_symbol symbol with attributre extern
 * @return error if symbol alredy defined as extern or the symbol is used as label elsewhere
 */
status_e symbol_handle_data_or_string(symbol *old_symbol, symbol *new_symbol) ;
/**
 * @brief prints error messeges
 * @param status error code
 */
void symbol_error_handle(status_e status) ;
/**
 * @brief assign a symbol memory if it is not assigned alredy
 * @param s symbol without an adresss
 * @param address assigning address
 * @return returns the size of command
 */
uint16_t symbol_assign_memory(symbol * s, int address);
/**
 * @brief checks if symbol is of type entery and if it was not initilized in a file
 * @param s symbol
 * @return status if it was initilized or not
 */
status_e symbol_is_uninitilized_entery(symbol * s);
/**
 * @brief gets the next avlible address form instructions list
 * @param instruction_data_table list of instructions
 * @param instruction_num amont of instructions
 * @return retutn the next avlible address or 100 if there is not instructions
 */
int symbol_get_varibles_start_address(instruction_data *instruction_data_table, int instruction_num);
/**
 * @brief incrise the size of symbol table by one
 * @param p_symbol_table pointer to symbol table
 * @param p_symbol_counter pointer to symbol counter
 * @return status if allocation was succssful
 */
status_e symbol_table_grow(symbol **p_symbol_table, int *p_symbol_counter);
/**
 * @brief duplicates a string
 * @param src string to copy
 * @return copy of a stringS
 */
char *my_strdup(const char *src);
#endif 

