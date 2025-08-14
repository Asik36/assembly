#include <stdio.h>
#include <main.h>
#include "machine_code.h"
#include "preassembly.h"
#include "sentence.h"
#include "memory_manager.h"
#include "symbol.h"
#include "errors.h"

int main(void)
{
    instruction * instruction_list = NULL;
    instruction_data  * instruction_data_list = NULL;
    int instruction_counter = 0;

    directive * directive_list = NULL;
    int directive_counter = 0;

    symbol * symbol_list = NULL;
    int symbol_counter = 0;

    machine_code * machine_code_list = NULL;
    int machine_code_counter = 0;

    list_macros("test.as");
    if(create_file_am("test.as","test.am") == FAILURE)
    {
        fprintf(stderr,"MACRO ERROR\n");
    }
    else if(sentence_decider("test.as",&instruction_list,&directive_list,&directive_counter,&instruction_counter) == FAILURE)
    {
        fprintf(stderr,"SENTENCE ERROR\n");
    }
    else if(memory(instruction_list,instruction_counter,&instruction_data_list) == FAILURE)
    {
        fprintf(stderr,"MEMORY ERROR\n");
    }




    
    for (int i = 0; i < directive_counter; i++)
    {
        if(directive_list != NULL && directive_list[i].data != NULL)
        {
           free(directive_list[i].data);
        }
    }

    free(instruction_list);
    free(directive_list);
    free(instruction_data_list);

    return 0;
}










