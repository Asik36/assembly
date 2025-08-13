#include <stdio.h>
#include "main.h"
#include "symbol.h"

status_e symbol_create(instruction_data *instruction_data_table, instruction *instruction_table, int instruction_num,
                       directive *directive_table, int directive_num,
                       symbol **symbol_table, int *symbol_num)
{
    status_e retval = STATUS_SUCCESS;
    int symbol_counter = 0;
    int start_address;
    *symbol_table = (symbol *)malloc(sizeof(symbol));


    start_address = symbol_get_varibles_start_address(instruction_data_table, instruction_num);
    printf("START ADDRESS : %d\n",start_address);
    hcreate(MEMORY_MAX_SIZE);
    

    /*instructions symbols */
    retval = symbol_instructions(instruction_data_table, instruction_table, instruction_num, symbol_table, &symbol_counter);
    /*directive symbols*/
    if (retval == STATUS_SUCCESS)
    {
        retval = symbol_directive(directive_table, directive_num, symbol_table, &symbol_counter);

    }
    if (retval == STATUS_SUCCESS)
    {
        retval = symbol_complete_table(*symbol_table, symbol_counter, start_address);
    }
    hdestroy();
    *symbol_num = symbol_counter;
    return retval;
}
int symbol_get_varibles_start_address(instruction_data *instruction_data_table, int instruction_num)
{
    int start_address = START_ADDRESS;
    if (instruction_num != 0)
    {
        start_address = instruction_data_table[instruction_num-1].address + instruction_data_table[instruction_num-1].size;
    }
    return start_address;
}

status_e symbol_instructions(instruction_data *instruction_data_table, instruction *instruction_table, int instruction_num,
                             symbol **p_symbol_table, int *p_symbol_counter)
{
    status_e retval = STATUS_SUCCESS;
    int symbol_counter = *p_symbol_counter;
    int i;
    symbol * tmp;
    symbol * symbol_table = *p_symbol_table;

    for (i = 0; i < instruction_num && (retval == STATUS_SUCCESS || retval == STATUS_NO_LABEL); i++)
    {

        if (retval != STATUS_NO_LABEL)
        {
            tmp = (symbol*) realloc((symbol*) symbol_table, (symbol_counter + 1) * sizeof(symbol));
            if(tmp == NULL)
            {
                fprintf(stderr,"reallocation failed\n");
                exit(1);
            }
            else
            {
            
                memset(&tmp[symbol_counter], 0, sizeof(symbol));
                symbol_counter++;

            }

        }

        retval = symbol_get_instruction_labels(&instruction_table[i], &instruction_data_table[i], &tmp[symbol_counter]);
        symbol_error_handle(retval);
    }
    *p_symbol_counter = symbol_counter;
    if (retval > 0)
    {
        retval = STATUS_SUCCESS;
    }
    return retval;
}
status_e symbol_directive(directive *directive_table, int directive_num,
                          symbol **p_symbol_table, int *p_symbol_counter)
{
    status_e retval = STATUS_SUCCESS;
    int symbol_counter = *p_symbol_counter;
    symbol * tmp;
    symbol * symbol_table = *p_symbol_table;
    int i = 0;
    for (i = 0; i < directive_num && retval < 0; i++)
    {
        if (retval == STATUS_SYMBOL_ENTER)
        {
            tmp = (symbol*) realloc((symbol*) symbol_table, (symbol_counter + 1) * sizeof(symbol));
            if(tmp == NULL)
            {
                fprintf(stderr,"reallocation failed\n");
                exit(1);
            }
            else
            {
                memset(&tmp[symbol_counter], 0, sizeof(symbol));
                symbol_counter++;

            }

        }
        retval = symbol_get_directive_labels(&directive_table[i], &tmp[symbol_counter]);
        symbol_error_handle(retval);
    }
    *p_symbol_counter = symbol_counter;
    if (retval > 0)
    {
        retval = STATUS_SUCCESS;
    }
    return retval;
}

status_e symbol_complete_table(symbol *symbol_table, int symbol_counter, int start_address)
{

    status_e retval = STATUS_SUCCESS;
    int current_address = start_address;
    int i;
    printf("CONTER : %d \n",symbol_counter);
    for (i = 0; i < symbol_counter && retval == STATUS_SUCCESS; i++)
    {
        //printf("NAME : %s\n",symbol_table[0].name);
        current_address += symbol_assign_memory(&symbol_table[i], current_address);
        retval = symbol_is_uninitilized_entery(&symbol_table[i]);
    }
    symbol_error_handle(retval);

    if (retval > 0)
    {
        retval = STATUS_SUCCESS;
    }
    return retval;
}
uint16_t symbol_assign_memory(symbol *s, int address)
{
    if (s->address == 0)
    {
        s->address = address;
    }
    return s->size;
}
status_e symbol_is_uninitilized_entery(symbol *s)
{
    status_e retval = STATUS_SUCCESS;
    if (s->access_attribute == ATTRIBUTE_ENTERY && s->size == 0)
    {
        retval = STATUS_ERR_UNINITILIZED_ENTRY;
    }
    return retval;
}

status_e symbol_get_instruction_labels(instruction *ins, instruction_data *ins_data, symbol *new_symbol)
{
    status_e retval = STATUS_SUCCESS;

    if (ins->label[0] != '\0')
    {
        strncpy(new_symbol->name, ins->label, SYMBOL_MAX_SIZE);
        new_symbol->address = ins_data->address;
        new_symbol->data_attribute = ATTRIBUTE_CODE;
        if (symbol_enter(new_symbol) == NULL)
        {
            retval = STATUS_ERR_DATA_ALREADY_DEFINED;
        }
        else
        {
            retval = STATUS_NO_LABEL;
        }
    }
    return retval;
}
status_e symbol_get_directive_labels(directive *dir, symbol *new_symbol)
{
    status_e retval;
    symbol *old_symbol = symbol_find(dir->variable_name);

    symbol_extract_directive(new_symbol, dir); /* copy data from dir to symbol */

    if (old_symbol == NULL)
    {
        symbol_enter(new_symbol);
        retval = STATUS_SYMBOL_UPDATE;
    }
    else
    {
        retval = symbol_update(old_symbol, new_symbol);
    }

    return retval;
}

void symbol_extract_directive(symbol *new_symbol, directive *dir)
{
    strncpy(new_symbol->name, dir->variable_name, SYMBOL_MAX_SIZE);
    new_symbol->access_attribute = dir->access_attribute;
    new_symbol->data_attribute = dir->data_attribute;
    new_symbol->size = dir->data_length;
    new_symbol->data = dir->data;
}

symbol *symbol_find(char *symbol_name)
{
    ENTRY item;
    ENTRY *found;
    item.key = symbol_name;
    found = hsearch(item, FIND);
    if (found == NULL)
    {
        printf("Symbol [%s] not found\n", symbol_name);
    }
    return found->data;
}
symbol *symbol_enter(symbol *s)
{
    ENTRY item;
    ENTRY *found;

    item.key = s->name;
    item.data = (void *)s;
    found = hsearch(item, ENTER);
    if (found == NULL)
    {
        printf("Symbol [%s] entered\n", s->name);
    }



    return (symbol* )found->data;
}

status_e symbol_handle_extern_attribute(symbol *old_symbol)
{
    status_e retval = STATUS_SUCCESS;

    if (old_symbol->access_attribute != ATTRIBUTE_EXTERN)
    {
        retval = STATUS_ERR_EXTERN_DEFINED_IN_FILE;
    }

    return retval;
}

status_e symbol_handle_entry_attribute(symbol *old_symbol)
{
    status_e retval = STATUS_SUCCESS;

    if (old_symbol->access_attribute == ATTRIBUTE_EXTERN)
    {
        retval = STATUS_ERR_ALREADY_EXTERN;
    }
    else
    {
        old_symbol->access_attribute = ATTRIBUTE_ENTERY;
    }

    return retval;
}

status_e symbol_handle_data_or_string(symbol *old_symbol, symbol *new_symbol)
{
    status_e retval = STATUS_SUCCESS;

    if (old_symbol->access_attribute == ATTRIBUTE_EXTERN)
    {
        retval = STATUS_ERR_ALREADY_EXTERN;
    }
    else if (old_symbol->size != 0 || old_symbol->data_attribute == ATTRIBUTE_CODE)
    {
        retval = STATUS_ERR_DATA_ALREADY_DEFINED;
    }
    else
    {
        old_symbol->data_attribute = new_symbol->data_attribute;
        old_symbol->size = new_symbol->size;
        old_symbol->data = new_symbol->data;
    }
    return retval;
}

/* Main dispatcher */
status_e symbol_update(symbol *old_symbol, symbol *new_symbol)
{
    status_e retval = STATUS_SYMBOL_UPDATE;

    if (new_symbol->access_attribute == ATTRIBUTE_EXTERN)
    {
        retval = symbol_handle_extern_attribute(old_symbol);
    }
    else if (new_symbol->access_attribute == ATTRIBUTE_ENTERY)
    {
        retval = symbol_handle_entry_attribute(old_symbol);
    }
    else if (new_symbol->data_attribute == ATTRIBUTE_DATA || new_symbol->data_attribute == ATTRIBUTE_STRING)
    {
        retval = symbol_handle_data_or_string(old_symbol, new_symbol);
    }
    return retval;
}

void symbol_error_handle(status_e status)
{
    switch (status)
    {
    case STATUS_SUCCESS:
        break;
    case STATUS_ERR_EXTERN_DEFINED_IN_FILE:
        fprintf(stderr, "ERROR : Extern can't be defined in a file.\n");
        break;
    case STATUS_ERR_ALREADY_EXTERN:
        fprintf(stderr, "ERROR : The Variable is already defined as extern.\n");
        break;
    case STATUS_ERR_DATA_ALREADY_DEFINED:
        fprintf(stderr, "ERROR : Multiple labels with the same name\n");
        break;
    case STATUS_ERR_UNINITILIZED_ENTRY:
        fprintf(stderr, "ERROR : Entery is not uninitilized\n");
        break;
    default:
        break;
    }
}

/**
 *
 * STR : mov a,b
 *
 *
 *
 *
 *
 *
 */