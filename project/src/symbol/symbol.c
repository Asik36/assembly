#include "symbol_t.h"

static int hasmap_index = 0;
static ENTRY *hashmap_data[MEMORY_MAX_SIZE] = {0};

symbol_status symbol_create(instruction_data_t *instruction_data_table, instruction_t *instruction_table, int instruction_num,
                       directive_t *directive_table, int directive_num,
                       symbol_t **symbol_table, int *symbol_num)
{
    symbol_status retval = SYMBOL_STATUS_SUCCESS;
    int starter_address;
    hcreate(MEMORY_MAX_SIZE);

    if (is_error(retval = symbol_instructions(instruction_data_table, instruction_table, instruction_num, symbol_table, symbol_num)))
    {
        symbol_error_handle(retval);
    }
    else if (is_error(retval = symbol_directive(directive_table, directive_num, symbol_table, symbol_num)))
    {
        symbol_error_handle(retval);
    }
    else
    {
        starter_address = symbol_get_varibles_start_address(instruction_data_table, instruction_num);
        symbol_complete_table(*symbol_table, *symbol_num, starter_address);
    }

    symbol_free_hashmap();

    return retval;
}

int symbol_get_varibles_start_address(instruction_data_t *instruction_data_table, int instruction_num)
{
    int start_address = START_ADDRESS;
    if (instruction_num != 0)
    {
        start_address = instruction_data_table[instruction_num - 1].address + instruction_data_table[instruction_num - 1].size;
    }
    return start_address;
}

symbol_status symbol_instructions(instruction_data_t *instruction_data_table, instruction_t *instruction_table, int instruction_num,
                             symbol_t **p_symbol_table, int *p_symbol_counter)
{
    symbol_status retval = SYMBOL_STATUS_SUCCESS;
    int i;
    instruction_data_t * current_instruction_data;
    instruction_t      * current_instruction;

    for (i = 0; i < instruction_num && !is_error(retval); i++)
    {
        /*increase the size of tree by one if the previously symbol was added to the list*/
        if (retval != SYMBOL_STATUS_NO_LABEL)
        {
            retval = symbol_table_grow(p_symbol_table, p_symbol_counter);
        }
        if (!is_error(retval))
        {
            current_instruction = instruction_table + i;
            current_instruction_data = instruction_data_table + i;
            retval = symbol_get_instruction_labels(current_instruction,current_instruction_data , *p_symbol_table, *p_symbol_counter - 1);
        }
    }
    if (retval == SYMBOL_STATUS_NO_LABEL)
    {
        symbol_table_shrink(p_symbol_table,p_symbol_counter);
    }


    return retval;
}


symbol_status symbol_directive(directive_t *directive_table, int directive_num,
                          symbol_t **p_symbol_table, int *p_symbol_counter)
{
    symbol_status retval = SYMBOL_STATUS_SUCCESS;

    directive_t *cur_directive;
    int i;
    for (i = 0; i < directive_num && !is_error(retval); i++)
    {
        /*increase the size of tree by one if the previously symbol was added to the list*/
        if (retval != SYMBOL_STATUS_SYMBOL_UPDATE)
        {
            retval = symbol_table_grow(p_symbol_table, p_symbol_counter);
        }
        if (!is_error(retval))
        {
            cur_directive = directive_table + i;
            retval = symbol_get_directive_labels(cur_directive, *p_symbol_table, *p_symbol_counter - 1);
        }
    }
    if (retval == SYMBOL_STATUS_SYMBOL_UPDATE)
    {
        symbol_table_shrink(p_symbol_table,p_symbol_counter);
    }


    return retval;
}


symbol_status symbol_get_instruction_labels(instruction_t *ins, instruction_data_t *ins_data, symbol_t *symbol_table, int index)
{
    symbol_status retval = SYMBOL_STATUS_SUCCESS;
    symbol_t *new_symbol = symbol_table + index;
    if (ins->label[0] == '\0')
    {
        retval = SYMBOL_STATUS_NO_LABEL;
    }
    else
    {
        symbol_extract_instruction(new_symbol,ins,ins_data); /*copies data from instruction to symbol*/
        if (symbol_find(new_symbol->name) != NULL)
        {
            retval = SYMBOL_STATUS_ERR_DATA_ALREADY_DEFINED;
        }
        else
        {
            symbol_enter(new_symbol->name, index);
        }
    }
    return retval;
}
symbol_status symbol_get_directive_labels(directive_t *dir, symbol_t *symbol_table, int index)
{
    symbol_status retval;
    int *p_old_symbol_index = symbol_find(dir->variable_name);
    symbol_t *new_symbol = symbol_table + index;
    symbol_t *old_symbol = NULL;
    symbol_extract_directive(new_symbol, dir); /* copy data from dir to symbol */

    /* if no label save in list but not in hashmap */
    if (*dir->variable_name == '\0')
    {
        retval = SYMBOL_STATUS_SYMBOL_ENTER;
    }
    /* enter new label into a list */
    else if (p_old_symbol_index == NULL)
    {
        symbol_enter(new_symbol->name, index);
        retval = SYMBOL_STATUS_SYMBOL_ENTER;
    }
    /* update found label in the list */
    else
    {
        old_symbol = symbol_table + *p_old_symbol_index;
        if (old_symbol != NULL)
        {
            retval = symbol_update(old_symbol, new_symbol);
        }
    }

    return retval;
}

void symbol_extract_directive(symbol_t *new_symbol, directive_t *dir)
{
    strncpy(new_symbol->name, dir->variable_name, SYMBOL_MAX_SIZE);
    new_symbol->access_attribute = dir->access_attribute;
    new_symbol->data_attribute = dir->data_attribute;
    new_symbol->size = dir->data_length;
    new_symbol->data = dir->data;
}
void symbol_extract_instruction(symbol_t *new_symbol, instruction_t *ins,instruction_data_t * ins_data)
{
    strncpy(new_symbol->name, ins->label, SYMBOL_MAX_SIZE);
    new_symbol->address = ins_data->address;
    new_symbol->data_attribute = ATTRIBUTE_CODE;
}

symbol_status symbol_complete_table(symbol_t *symbol_table, int symbol_counter, int start_address)
{

    symbol_status retval = SYMBOL_STATUS_SUCCESS;
    int current_address = start_address;
    int i;
    for (i = 0; i < symbol_counter && retval == SYMBOL_STATUS_SUCCESS; i++)
    {
        current_address += symbol_assign_memory(&symbol_table[i], current_address);
        retval = symbol_is_uninitilized_entery(&symbol_table[i]);
        if (current_address > MEMORY_MAX_SIZE)
        {
            retval = SYMBOL_STATUS_ERR_MEMORY_OVERFLOW;
        }
    }
    if (is_error(retval))
    {
        symbol_error_handle(retval);
    }
    return retval;
}
uint16_t symbol_assign_memory(symbol_t *s, int address)
{
    if (s->access_attribute == ATTRIBUTE_EXTERN)
    {
        s->address = 0;
    }
    else if (s->data_attribute != ATTRIBUTE_CODE && s->size != 0)
    {
        s->address = address;
    }
    return s->size;
}
symbol_status symbol_is_uninitilized_entery(symbol_t *s)
{
    symbol_status retval = SYMBOL_STATUS_SUCCESS;
    if (s->access_attribute == ATTRIBUTE_ENTERY && s->data_attribute == ATTRIBUTE_DATA_NONE)
    {
        retval = SYMBOL_STATUS_ERR_UNINITILIZED_ENTRY;
    }
    return retval;
}

int *symbol_find(char *symbol_name)
{
    ENTRY item;
    ENTRY *found = NULL;
    int *index = NULL;
    if (symbol_name != NULL && *symbol_name != '\0')
    {
        item.key = symbol_name;
        found = hsearch(item, FIND);
    }

    if (found != NULL)
    {
        index = (int *)found->data;
    }

    return index;
}
int symbol_enter(char *symbol_name, int index)
{
    ENTRY item;
    ENTRY *found = NULL;
    int *p_index = malloc(sizeof(int));
    *p_index = index;
    item.key = my_strdup(symbol_name);
    item.data = p_index;
    found = hsearch(item, ENTER);
    if (found != NULL)
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

symbol_status symbol_update(symbol_t *old_symbol, symbol_t *new_symbol)
{
    symbol_status retval = SYMBOL_STATUS_SYMBOL_UPDATE;

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

symbol_status symbol_handle_extern_attribute(symbol_t *old_symbol)
{
    symbol_status retval = SYMBOL_STATUS_SYMBOL_UPDATE;

    if (old_symbol->access_attribute != ATTRIBUTE_EXTERN)
    {
        retval = SYMBOL_STATUS_ERR_EXTERN_DEFINED_IN_FILE;
    }

    return retval;
}

symbol_status symbol_handle_entry_attribute(symbol_t *old_symbol)
{
    symbol_status retval = SYMBOL_STATUS_SYMBOL_UPDATE;

    if (old_symbol->access_attribute == ATTRIBUTE_EXTERN)
    {
        retval = SYMBOL_STATUS_ERR_ALREADY_EXTERN;
    }
    else
    {
        old_symbol->access_attribute = ATTRIBUTE_ENTERY;
    }

    return retval;
}

symbol_status symbol_handle_data_or_string(symbol_t *old_symbol, symbol_t *new_symbol)
{
    symbol_status retval = SYMBOL_STATUS_SYMBOL_UPDATE;

    if (old_symbol->access_attribute == ATTRIBUTE_EXTERN)
    {
        retval = SYMBOL_STATUS_ERR_ALREADY_EXTERN;
    }
    else if (old_symbol->size != 0 || old_symbol->data_attribute == ATTRIBUTE_CODE)
    {
        retval = SYMBOL_STATUS_ERR_DATA_ALREADY_DEFINED;
    }
    else
    {
        old_symbol->data_attribute = new_symbol->data_attribute;
        old_symbol->size = new_symbol->size;
        old_symbol->data = new_symbol->data;
    }
    return retval;
}


symbol_status symbol_table_grow(symbol_t **p_symbol_table, int *p_symbol_counter)
{
    symbol_t *tmp = NULL;
    int symbol_counter = *p_symbol_counter;
    symbol_t *symbol_table = *p_symbol_table;
    symbol_status retval = SYMBOL_STATUS_SUCCESS;

    tmp = (symbol_t *)realloc(symbol_table, (symbol_counter + 1) * sizeof(symbol_t));
    if (tmp == NULL)
    {
        retval = SYMBOL_STATUS_ERR_MEMORY_ALLOCATION;
    }
    else
    {
        symbol_table = tmp;
        memset(symbol_table + symbol_counter, 0, sizeof(symbol_t));

        symbol_counter++;

        *p_symbol_table = symbol_table;
        *p_symbol_counter = symbol_counter;
    }

    return retval;
}

symbol_status symbol_table_shrink(symbol_t **p_symbol_table,int *p_symbol_counter)
{
    symbol_status retval = SYMBOL_STATUS_SUCCESS;
    int symbol_counter = *p_symbol_counter;
    symbol_t *symbol_table = *p_symbol_table;

    symbol_counter--;
    if (symbol_counter != 0)
    {
        symbol_table = realloc(symbol_table, symbol_counter * sizeof(symbol_t));
            if (symbol_table == NULL)
            {
                retval = SYMBOL_STATUS_ERR_MEMORY_ALLOCATION;
            }
    }

    *p_symbol_counter = symbol_counter;
    *p_symbol_table = symbol_table;
    return retval;
}


void symbol_free_hashmap(void)
{
    for (int i = 0; i < hasmap_index; i++)
    {
        if (hashmap_data[i]->data != NULL)
        {
            free(hashmap_data[i]->data);
        }
        if (hashmap_data[i]->key != NULL)
        {
            free(hashmap_data[i]->key);
        }
    }
    hdestroy();
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

void symbol_error_handle(symbol_status status)
{
    switch (status)
    {
    case SYMBOL_STATUS_SUCCESS:
        break;
    case SYMBOL_STATUS_ERR_EXTERN_DEFINED_IN_FILE:
        fprintf(stderr, MSG_ERR_EXTERN_DEFINED_IN_FILE);
        break;
    case SYMBOL_STATUS_ERR_ALREADY_EXTERN:
        fprintf(stderr, MSG_ERR_ALREADY_EXTERN);
        break;
    case SYMBOL_STATUS_ERR_DATA_ALREADY_DEFINED:
        fprintf(stderr, MSG_ERR_DATA_ALREADY_DEFINED);
        break;
    case SYMBOL_STATUS_ERR_UNINITILIZED_ENTRY:
        fprintf(stderr, MSG_ERR_UNINITILIZED_ENTRY);
        break;
    case SYMBOL_STATUS_ERR_MEMORY_ALLOCATION:
        fprintf(stderr, MSG_ERR_MEMORY_ALLOCATION);
        break;
    case SYMBOL_STATUS_ERR_MEMORY_OVERFLOW:
        fprintf(stderr, MSG_ERR_MEMORY_OVERFLOW);
        break;
    default:
        fprintf(stderr, MSG_ERR_UNDEFINED);
        break;
    }
}


