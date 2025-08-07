#include <stdio.h>
#include <main.h>
#include "symbol.h"
int symbol_num = 0;
int instruction_counter = 0;


status symbol_create(   instruction * instruction_table, int instruction_num,
                        directive   * directive_table  , int directive_num  ,
                        symbol      * symbol_table                          )
{



}
void symbol_read_instruction_table(instruction * instruction_table,int instruction_num,symbol * symbol_table )
{
    symbol * current_symbol;
    for (int i = 0; i < instruction_num; i++)
    {
        /*label handle*/
        if(instruction_table[i].label != 0)
        {
            current_symbol = symbol_get_by_name(symbol_table,instruction_table[i].label,SYMBOL_MAX_SIZE);
        }
        if(current_symbol != NULL)
        {

        }


    }

}



symbol * symbol_get_by_name(symbol * symbol_table, char * name, int name_length)
{
    status return_status = FAILURE;
    symbol * selected_symbol = NULL;
    int i;
    for (i = 0; i < symbol_num; i++)
    {
        if(strncmp(symbol_table[i].name,name,name_length) == 0)
        {
            selected_symbol = &symbol_table[i];

        }
    }


    return selected_symbol;

}



void symbol_assign_memmory_address(instruction * ins)
{


}

int symbol_get_instruction_offset(instruction * ins)
{
    int offset = 0;
    if(ins->src_operand == NULL && ins->dest_operand == NULL)
    {
        offset = 1;
    }
    else
    {

    }
    return offset;
}

addressing_modes symbol_get_address_mode(char * operand)
{
    addressing_modes mode;

    if(*operand == '#')
    {
        mode = ADDRESSING_MODES_IMMEDIATE;
    }
    else if(symbol_is_operand(operand))
    {

    }

}

bool symbol_is_operand(char * operand)
{
    int i;
    bool flag = false;
    for (i = 0; i < REGISTER_AMONT && flag == false; i++)
    {
        flag = strncmp(operand,register_names[i],SYMBOL_MAX_SIZE) == 0;
    }

    return flag;
}

