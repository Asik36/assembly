#ifndef SYMBOL_H
#define SYMBOL_H
#include <stdio.h>
#include <string.h>

status symbol_create(   instruction * instruction_table, int instruction_num,
                        directive   * directive_table  , int directive_num  ,
                        symbol      * symbol_table                          );

void symbol_assign_memmory_address(instruction * ins);

symbol * symbol_get_by_name(symbol * symbol_table, char * name, int name_length);

void symbol_read_instruction_table(instruction * instruction_table,int instruction_num,symbol * symbol_table);


uint16_t symbol_get_instruction_offset(instruction * ins);

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