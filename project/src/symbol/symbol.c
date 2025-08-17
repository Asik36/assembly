#include "symbol.h"

static int hasmap_index = 0;
static ENTRY * hashmap_data[MEMORY_MAX_SIZE] = {0};

status_e symbol_create(instruction_data *instruction_data_table, instruction *instruction_table, int instruction_num,
                       directive *directive_table, int directive_num,
                       symbol **symbol_table, int *symbol_num)
{
    status_e retval = STATUS_SUCCESS;
    int starter_address;
    hcreate(MEMORY_MAX_SIZE);
    
    *symbol_table = NULL;
    if ((retval = symbol_instructions(instruction_data_table, instruction_table, instruction_num, symbol_table, symbol_num)) < STATUS_SUCCESS)
    {
        symbol_error_handle(retval);  
    }
    else if((retval = symbol_directive(directive_table, directive_num, symbol_table, symbol_num)) < STATUS_SUCCESS)
    {
        symbol_error_handle(retval);  
    }
    else 
    {
        starter_address = symbol_get_varibles_start_address(instruction_data_table,instruction_num);
        symbol_complete_table(*symbol_table,*symbol_num,starter_address);
    }
    for (int i = 0; i < hasmap_index; i++)
    {

        if(hashmap_data[i]->data != NULL)
        {
            free(hashmap_data[i]->data);

        }
        if(hashmap_data[i]->key != NULL)
        {
            free(hashmap_data[i]->key);
        }
    }
    hdestroy();

    return retval;
}
int symbol_get_varibles_start_address(instruction_data *instruction_data_table, int instruction_num)
{
    int start_address = START_ADDRESS;
    if (instruction_num != 0)
    {
        start_address = instruction_data_table[instruction_num - 1].address + instruction_data_table[instruction_num - 1].size;
    }
    return start_address;
}

status_e symbol_instructions(instruction_data *instruction_data_table, instruction *instruction_table, int instruction_num,
                             symbol **p_symbol_table, int *p_symbol_counter)
{
    status_e retval = STATUS_SUCCESS;
    int symbol_counter = *p_symbol_counter;
    symbol *symbol_table = *p_symbol_table;
    int i;
    for (i = 0; i < instruction_num && !is_error(retval); i++)
    {
        if (retval != STATUS_NO_LABEL)
        {
            retval = symbol_table_grow(&symbol_table, &symbol_counter);
    
        }
        if (!is_error(retval))
        {
            retval = symbol_get_instruction_labels(instruction_table + i, instruction_data_table + i, symbol_table,symbol_counter-1);
        }
        
    }
    if(retval == STATUS_NO_LABEL)
    {   
        symbol_counter--;
        if(symbol_counter != 0)
        {
            symbol_table = realloc(symbol_table,symbol_counter * sizeof(symbol));
        }
    }
    *p_symbol_counter = symbol_counter;
    *p_symbol_table = symbol_table;

    return retval;
}

status_e symbol_directive(directive *directive_table, int directive_num,
                          symbol **p_symbol_table, int *p_symbol_counter)
{
    status_e retval = STATUS_SUCCESS;
    int symbol_counter = *p_symbol_counter;
    symbol *symbol_table = *p_symbol_table;
    directive *cur_directive;
    int i;
    for (i = 0; i < directive_num && !is_error(retval); i++)
    {
        if (retval != STATUS_SYMBOL_UPDATE)
        {
            retval = symbol_table_grow(&symbol_table, &symbol_counter);

        }
        if (!is_error(retval))
        {
            cur_directive = directive_table + i;
            retval = symbol_get_directive_labels(cur_directive, symbol_table,symbol_counter-1);
        }
      
    }
    if(retval == STATUS_SYMBOL_UPDATE)
    {
        symbol_counter--;
        symbol_table = realloc(symbol_table,(symbol_counter) * sizeof(symbol));
    }
    *p_symbol_counter = symbol_counter;
    *p_symbol_table = symbol_table;

    return retval;
}

status_e symbol_complete_table(symbol *symbol_table, int symbol_counter, int start_address)
{

    status_e retval = STATUS_SUCCESS;
    int current_address = start_address;
    int i;
    for (i = 0; i < symbol_counter && retval == STATUS_SUCCESS; i++)
    {
        current_address += symbol_assign_memory(&symbol_table[i], current_address);
        retval = symbol_is_uninitilized_entery(&symbol_table[i]);
        if(current_address > MEMORY_MAX_SIZE)
        {
            retval = STATUS_ERR_MEMORY_OVERFLOW;
        }
    }
    if (!is_error(retval))
    {
        retval = STATUS_SUCCESS;
    }
    else
    {
        symbol_error_handle(retval);
    }
    return retval;
}
uint16_t symbol_assign_memory(symbol *s, int address)
{
    if(s->access_attribute == ATTRIBUTE_EXTERN)
    {
        s->address = 0;
    }
    else if (s->data_attribute != ATTRIBUTE_CODE && s->size != 0)
    {
        s->address = address;
    }
    return s->size;
}
status_e symbol_is_uninitilized_entery(symbol *s)
{
    status_e retval = STATUS_SUCCESS;
    if (s->access_attribute == ATTRIBUTE_ENTERY && s->data_attribute == ATTRIBUTE_DATA_NONE)
    {
        retval = STATUS_ERR_UNINITILIZED_ENTRY;
    }
    return retval;
}

status_e symbol_get_instruction_labels(instruction *ins, instruction_data *ins_data, symbol *symbol_table,int index)
{
    status_e retval = STATUS_SUCCESS;
    symbol * new_symbol = symbol_table+index;
    if (ins->label[0] != '\0')
    {
        strncpy(new_symbol->name, ins->label, SYMBOL_MAX_SIZE);
        new_symbol->address = ins_data->address;
        new_symbol->data_attribute = ATTRIBUTE_CODE;
        if (symbol_find(new_symbol->name) != NULL)
        {
            
            retval = STATUS_ERR_DATA_ALREADY_DEFINED;
        }
        else
        {
            symbol_enter(new_symbol->name,index);
        }
    }
    else
    {
        retval = STATUS_NO_LABEL;
    }
    return retval;
}
status_e symbol_get_directive_labels(directive *dir, symbol *symbol_table,int index)
{
    status_e retval;
    int * p_old_symbol_index = symbol_find(dir->variable_name);
    symbol * new_symbol = symbol_table+index;
    symbol * old_symbol = NULL;
    symbol_extract_directive(new_symbol, dir); /* copy data from dir to symbol */

    /* if no label save in list but not in hashmap */
    if(*dir->variable_name == '\0')
    {
        retval = STATUS_SYMBOL_ENTER;
    }
    /* enter new label into a list */
    else if (p_old_symbol_index == NULL)
    {

        symbol_enter(new_symbol->name,index);
        retval = STATUS_SYMBOL_ENTER;
    }
    /* update found label in the list */
    else
    {
        old_symbol = symbol_table+*p_old_symbol_index;
        if(old_symbol != NULL)
        {
            retval = symbol_update(old_symbol, new_symbol);
        }

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

int * symbol_find(char *symbol_name)
{
    ENTRY item;
    ENTRY *found = NULL;
    int * index = NULL;
    if(symbol_name != NULL && *symbol_name != '\0')
    {
        item.key = symbol_name;
        found = hsearch(item, FIND); 
    }

    if (found != NULL)
    {
        index = (int*)found->data;

    }

    return index;
}
int symbol_enter(char *symbol_name,int index)
{
    ENTRY item;
    ENTRY *found =NULL;
    int * p_index = malloc(sizeof(int));
    *p_index = index;
    item.key = my_strdup(symbol_name);
    item.data = p_index;
    found = hsearch(item, ENTER);
    if(found != NULL)
    {
        hashmap_data[hasmap_index] = found;
        hasmap_index++;
    }
    else
    {
        index = -1;
    }
    




    return index;
}

status_e symbol_handle_extern_attribute(symbol *old_symbol)
{
    status_e retval = STATUS_SYMBOL_UPDATE;

    if (old_symbol->access_attribute != ATTRIBUTE_EXTERN)
    {
        retval = STATUS_ERR_EXTERN_DEFINED_IN_FILE;
    }

    return retval;
}

status_e symbol_handle_entry_attribute(symbol *old_symbol)
{
    status_e retval = STATUS_SYMBOL_UPDATE;

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
    status_e retval = STATUS_SYMBOL_UPDATE;

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
        fprintf(stderr, MSG_ERR_EXTERN_DEFINED_IN_FILE);
        break;
    case STATUS_ERR_ALREADY_EXTERN:
        fprintf(stderr, MSG_ERR_ALREADY_EXTERN);
        break;
    case STATUS_ERR_DATA_ALREADY_DEFINED:
        fprintf(stderr, MSG_ERR_DATA_ALREADY_DEFINED);
        break;
    case STATUS_ERR_UNINITILIZED_ENTRY:
        fprintf(stderr, MSG_ERR_UNINITILIZED_ENTRY);
        break;
    case STATUS_ERR_MEMORY_ALLOCATION:
        fprintf(stderr, MSG_ERR_MEMORY_ALLOCATION);
        break;
    case STATUS_ERR_MEMORY_OVERFLOW:
        fprintf(stderr, MSG_ERR_MEMORY_OVERFLOW);
        break;
    default:
        fprintf(stderr, MSG_ERR_UNDEFINED);
        break;
    }
}

char *my_strdup(const char *src)
{
    char *dst = NULL;

    if (src != NULL)
    {
        size_t len = strlen(src) + 1; /* +1 for '\0' */
        dst = malloc(len);
        if (dst != NULL)
        {
            memcpy(dst, src, len);
        }
    }

    return dst;
}

status_e symbol_table_grow(symbol **p_symbol_table, int *p_symbol_counter)
{
    symbol *tmp = NULL;
    int symbol_counter = *p_symbol_counter;
    symbol *symbol_table = *p_symbol_table;
    status_e retval = STATUS_SUCCESS;

    tmp = (symbol *)realloc(symbol_table, (symbol_counter + 1) * sizeof(symbol));
    if (tmp == NULL)
    {
        fprintf(stderr, "Realloc error\n");
        retval = STATUS_ERR_MEMORY_ALLOCATION;
    }
    else
    {
        symbol_table = tmp;
        memset(symbol_table + symbol_counter, 0, sizeof(symbol));



        symbol_counter++;

        *p_symbol_table = symbol_table;
        *p_symbol_counter = symbol_counter;
    }

    return retval;
}
