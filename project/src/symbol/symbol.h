#ifndef SYMBOL_H
#define SYMBOL_H
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <search.h>
#define START_ADDRESS 100
typedef enum 
{
    STATUS_ERR_UNINITILIZED_ENTRY = -4,
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
 * @param p_symbol_table pointer to list of symbols
 * @param p_symbol_num pointer to number of symbols
 * @return return status that tells if symbol table created succssfully
 */
status_e symbol_create(instruction_data *instruction_data_table, instruction *instruction_table, int instruction_num,
                       directive *directive_table, int directive_num,
                       symbol **p_symbol_table, int *p_symbol_num);

status_e symbol_directive(directive *directive_table, int directive_num,
                          symbol **p_symbol_table, int *p_symbol_counter);

status_e symbol_complete_table(symbol * p_symbol_table, int symbol_counter, int start_address);

status_e symbol_instructions(   instruction_data * instruction_data_table, instruction * instruction_table, int instruction_num,
                                symbol           ** p_symbol_table          , int * p_symbol_counter);

status_e symbol_get_instruction_labels(instruction * ins,instruction_data * ins_data , symbol * new_symbol);

status_e symbol_get_directive_labels(directive * dir, symbol * new_symbol);

void symbol_extract_directive(symbol *new_symbol, directive *dir);

symbol * symbol_find(char * symbol_name);
symbol * symbol_enter(symbol * s);
status_e symbol_handle_extern_attribute(symbol *old_symbol);
status_e symbol_handle_entry_attribute(symbol *old_symbol) ;
status_e symbol_handle_data_or_string(symbol *old_symbol, symbol *new_symbol) ;
status_e symbol_update(symbol *old_symbol, symbol *new_symbol) ;
void symbol_error_handle(status_e status) ;

uint16_t symbol_assign_memory(symbol * s, int address);
status_e symbol_is_uninitilized_entery(symbol * s);

int symbol_get_varibles_start_address(instruction_data *instruction_data_table, int instruction_num);


#endif 

/*
 * macro m1
 * inc r2
 * endm
 * mov m1, r3
 * 
 * 
 * 
 *
 */