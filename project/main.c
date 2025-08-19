#include <stdio.h>
#include <common_structs.h>
#include "machine_code.h"
#include "preassembly.h"
#include "sentence.h"
#include "memory_manager.h"
#include "symbol.h"
#include "errors.h"
#define MAIN_MSG_ERR_INPUT_FILES   "ERROR : INPUT FILES\n"
#define MAIN_MSG_ERR_MACRO         "ERROR : MACRO\n"
#define MAIN_MSG_ERR_SENTENCE      "ERROR : SENTENCE\n"
#define MAIN_MSG_ERR_MEMORY        "ERROR : MEMORY\n"
#define MAIN_MSG_ERR_MACHINE_CODE  "ERROR : MACHINE CODE\n"
#define MAIN_MSG_ERR_OUTPUT_FILES  "ERROR : OUTPUT FILES\n"
#define MAIN_MSG_ERR_UNKNOWN       "ERROR : UNKNOWN\n"
#define ASSEMBLY_FILE_NAME_EXTENSION ".as"

int main(int argc, char **argv )
{
    main_status retval = SUCCESS;
    for (int i = 1; i < argc && retval == MAIN_STATUS_SUCCESS; i++)
    {
        retval = assembly(argv[i]);
    }
    main_error_check(retval);
    
    return retval;
}



main_status assembly(char * file_as)
{
    main_status retval = MAIN_STATUS_SUCCESS;

    instruction         * instruction_list = NULL;
    instruction_data    * instruction_data_list = NULL;
    directive           * directive_list = NULL;
    symbol              * symbol_list = NULL;

    int instruction_counter = 0;
    int directive_counter   = 0;
    int symbol_counter      = 0;

    char base_file_name[FILE_NAME_MAX_LEN] = {0};

    if(get_base_file_name(file_as,base_file_name) < 0)
    {
        retval = MAIN_STATUS_ERR_IN_FILES;
    }
    else if(check_processing_modules(file_as,base_file_name,&directive_list,&instruction_list,&instruction_counter,&directive_counter) < 0)
    {
        retval = MAIN_STATUS_ERR_MACRO;
    }
    else if(memory(instruction_list,instruction_counter,&instruction_data_list) < 0)
    {
        retval = MAIN_STATUS_ERR_MEMORY;
    }
    else if(symbol_create(instruction_data_list,instruction_list,instruction_counter,directive_list,directive_counter,&symbol_list,&symbol_counter) < 0)
    {
        retval = MAIN_STATUS_ERR_SYMBOL;
    }
    else if(machine_code_main(base_file_name,symbol_list,symbol_counter,instruction_data_list,instruction_counter)== false)
    {
        retval = MAIN_STATUS_ERR_MACHINE_CODE;
    }
    free_lists(directive_counter, directive_list, instruction_list, instruction_data_list, symbol_list);

    return retval;
}

status get_base_file_name(char * input_file , char * base_file_name)
{
    status retval = SUCCESS;
    char * ending = strstr(input_file,ASSEMBLY_FILE_NAME_EXTENSION);
    int file_len = 0;
    if(ending == NULL || *(ending + strlen(ASSEMBLY_FILE_NAME_EXTENSION)-1) == '\0')
    {
        retval = FAILURE;
    }
    else
    {
        file_len = ending - input_file;
        strncpy(base_file_name,input_file,file_len); 
        base_file_name[file_len] = '\0';
    }
    return retval;
}

void main_error_check(main_status m_status)
{
    switch (m_status)
    {
    case MAIN_STATUS_SUCCESS:
        break;
    case MAIN_STATUS_ERR_IN_FILES:
        fprintf(stderr, MAIN_MSG_ERR_INPUT_FILES);
        break;
    case MAIN_STATUS_ERR_MACRO:
        fprintf(stderr, MAIN_MSG_ERR_MACRO);
        break;
    case MAIN_STATUS_ERR_SENTENCE:
        fprintf(stderr, MAIN_MSG_ERR_SENTENCE);
        break;
    case MAIN_STATUS_ERR_MEMORY:
        fprintf(stderr, MAIN_MSG_ERR_MEMORY);
        break;
    case MAIN_STATUS_ERR_MACHINE_CODE:
        fprintf(stderr, MAIN_MSG_ERR_MACHINE_CODE);
        break;
    case MAIN_STATUS_ERR_OUT_FILES:
        fprintf(stderr, MAIN_MSG_ERR_OUTPUT_FILES);
        break;
    default:
        fprintf(stderr, MAIN_MSG_ERR_UNKNOWN);
        break;
    }
}

void free_lists(int directive_counter, directive *directive_list, instruction *instruction_list, instruction_data *instruction_data_list, symbol *symbol_list)
{
    for (int i = 0; i < directive_counter; i++)
    {
        if (directive_list != NULL && directive_list[i].data != NULL)
        {
            free(directive_list[i].data);
        }
    }

    free(instruction_list);
    free(directive_list);
    free(instruction_data_list);
    free(symbol_list);
}

