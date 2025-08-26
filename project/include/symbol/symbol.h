#ifndef SYMBOL_H
#define SYMBOL_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include "common_structs.h"
#define MSG_SUCCESS ""
#define MSG_ERR_EXTERN_DEFINED_IN_FILE "ERROR : Extern can't be defined in a file.\n"
#define MSG_ERR_ALREADY_EXTERN "ERROR : A entery symbol is already defined as extern.\n"
#define MSG_ERR_DATA_ALREADY_DEFINED "ERROR : Multiple labels with the same name\n"
#define MSG_ERR_UNINITILIZED_ENTRY "ERROR : Entry is uninitialized\n"
#define MSG_ERR_MEMORY_ALLOCATION "ERROR : Memory allocation failed\n"
#define MSG_ERR_MEMORY_OVERFLOW "ERROR : Address memory out of bounds\n"
#define MSG_ERR_UNDEFINED "ERROR : Undefined\n"
typedef enum symbol_status_e
{
    SYMBOL_STATUS_ERR_MEMORY_OVERFLOW = -6,
    SYMBOL_STATUS_ERR_MEMORY_ALLOCATION,
    SYMBOL_STATUS_ERR_UNINITILIZED_ENTRY,
    SYMBOL_STATUS_ERR_EXTERN_DEFINED_IN_FILE,
    SYMBOL_STATUS_ERR_ALREADY_EXTERN,
    SYMBOL_STATUS_ERR_DATA_ALREADY_DEFINED,
    SYMBOL_STATUS_SUCCESS = 0,
    SYMBOL_STATUS_NO_LABEL,
    SYMBOL_STATUS_SYMBOL_UPDATE,
    SYMBOL_STATUS_SYMBOL_ENTER

} symbol_status;

/**
 * @brief Creates symbol table from instructions and directive
 * @param instruction_data_table instruction data list
 * @param instruction_table instruction_t strings
 * @param instruction_num amont of instruction
 * @param directive_table list of directive_t sentence
 * @param directive_num amont of drrective sentences
 * @param p_symbol_table pointer to list of symbols
 * @param p_symbol_counter pointer to number of symbols
 * @return return status_e that tells if symbol table created succssfully or not
 */
symbol_status symbol_create(instruction_data_t *instruction_data_table, instruction_t *instruction_table, int instruction_num,
                       directive_t *directive_table, int directive_num,
                       symbol_t **p_symbol_table, int *p_symbol_counter);


/**
 *
 * @brief fill symbols from instruction sentences
 * @param instruction_data_table instruction data list
 * @param instruction_table instruction_t strings
 * @param instruction_num amont of instruction
 * @param p_symbol_table pointer to list of symbols
 * @param p_symbol_counter pointer to number of symbols
 * @return status_e type of error on symbol creation or succsess
 */
symbol_status symbol_instructions(instruction_data_t *instruction_data_table, instruction_t *instruction_table, int instruction_num,
                             symbol_t **p_symbol_table, int *p_symbol_counter);
/**
 * @brief fill symbols from directive sentences
 * @param directive_table list of directive_t sentence
 * @param directive_num amont of drrective sentences
 * @param p_symbol_table pointer to list of symbols
 * @param p_symbol_counter pointer to number of symbols
 * @return status_e type of error on symbol creation or succsess
 */
symbol_status symbol_directive(directive_t *directive_table, int directive_num,
                          symbol_t **p_symbol_table, int *p_symbol_counter);

/**
 * @brief fill the remaining unsigned address of symbols and checks for errors in the symbol table
 * @param p_symbol_table pointer to list of symbols
 * @param symbol_counter amont of symbols
 * @param start_address defualt start adress in that unisgined address and will start from
 * @return status_e type of error on existing symbol or succsess if no errors
 */
symbol_status symbol_complete_table(symbol_t *p_symbol_table, int symbol_counter, int start_address);

/**
 * @brief get label from instruction and add it to list if it does not exists. includes the address of the current instruction
 * @param ins current instruction_t
 * @param ins_data current instruction data
 * @param symbol_table list of symbols
 * @param index offset of the current symbol
 * @return return error if label alredy exists otherwise return succsess
 */
symbol_status symbol_get_instruction_labels(instruction_t *ins, instruction_data_t *ins_data, symbol_t *symbol_table, int index);

/**
 * @brief get label from directive and add it to list if. updatess data atterbiute or accsess attribute if alredy exists
 * @param dir current directive_t
 * @param symbol_table list of symbols
 * @param index offset of the current symbol
 * @return return code error if label alredy exists or is unvaild, otherwise return succsess
 */
symbol_status symbol_get_directive_labels(directive_t *dir, symbol_t *symbol_table, int index);

/**
 * @brief copys data from instruction into symbol
 * 
 * @param new_symbol destination
 * @param ins source instruction_t for symbol name
 * @param ins_data source instruction data for address
 */
void symbol_extract_instruction(symbol_t *new_symbol, instruction_t *ins,instruction_data_t * ins_data);

/**
 * @brief copys data from directive into symbol
 * @param new_symbol destination symbol
 * @param dir source directory
 */
void symbol_extract_directive(symbol_t *new_symbol, directive_t *dir);
/**
 * @brief search for symbol index inside a hashmap using symbol name
 * @param symbol_name key name for searching in hashmap
 * @return pointer to index of symbol table
 */
int *symbol_find(char *symbol_name);
/**
 * @brief enter a new index of symbol table into a hashmap
 * @param symbol_name key name for searching in hashmap
 * @return if symbol name does not exists returns its index, else returns -1
 */
int symbol_enter(char *symbol_name, int index);

/**
 * @brief copies the data from new_symbol to old symbol and checks for errors
 * @param old_symbol dest symbol
 * @param new_symbol src symbol
 * @return succsess or type of error
 */
symbol_status symbol_update(symbol_t *old_symbol, symbol_t *new_symbol);

/**
 * @brief checks for errors in case symbol has attribute extern
 * @param old_symbol symbol with attributre extern
 * @return error if extern is alredy defined inside the fille
 */
symbol_status symbol_handle_extern_attribute(symbol_t *old_symbol);
/**
 * @brief checks for errors in case symbol has entery extern
 * @param old_symbol symbol with attributre extern
 * @return error if symbol alredy defined as extern
 */
symbol_status symbol_handle_entry_attribute(symbol_t *old_symbol);
/**
 * @brief checks for errors in case symbol has entery data or string
 * @param old_symbol symbol with attributre extern
 * @return error if symbol alredy defined as extern or the symbol is used as label elsewhere
 */
symbol_status symbol_handle_data_or_string(symbol_t *old_symbol, symbol_t *new_symbol);
/**
 * @brief prints error messeges
 * @param status_e error code
 */
void symbol_error_handle(symbol_status status_e);
/**
 * @brief assign a symbol memory if it is not assigned alredy
 * @param s symbol without an adresss
 * @param address assigning address
 * @return returns the size of command
 */
uint16_t symbol_assign_memory(symbol_t *s, int address);
/**
 * @brief checks if symbol is of type entery and if it was not initilized in a file
 * @param s symbol
 * @return status_e if it was initilized or not
 */
symbol_status symbol_is_uninitilized_entery(symbol_t *s);
/**
 * @brief gets the next avlible address form instructions list
 * @param instruction_data_table list of instructions
 * @param instruction_num amont of instructions
 * @return retutn the next avlible address or 100 if there is not instructions
 */
int symbol_get_varibles_start_address(instruction_data_t *instruction_data_table, int instruction_num);
/**
 * @brief increase the size of symbol table by one
 * @param p_symbol_table pointer to symbol table
 * @param p_symbol_counter pointer to symbol counter
 * @return status_e SYMBOL_STATUS_ERR_MEMORY_ALLOCATION if allocation failed
 */
symbol_status symbol_table_grow(symbol_t **p_symbol_table, int *p_symbol_counter);

/**
 * @brief decreses the size of symbol table by one
 *
 * @param p_symbol_table pointer to symbol table
 * @param p_symbol_counter pointer to symbol counter
 * @return status_e SYMBOL_STATUS_ERR_MEMORY_ALLOCATION if allocation failed
 */
symbol_status symbol_table_shrink(symbol_t **p_symbol_table, int *p_symbol_counter);

/**
 * @brief duplicates a string
 * @param src string to copy
 * @return copy of a stringS
 */
char *my_strdup(const char *src);

/**
 * @brief free's hashmap keys, data and the hashmap itself
 *
 */
void symbol_free_hashmap(void);

#endif
