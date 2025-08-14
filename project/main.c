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



    if(check_processing_modules("test.as","test.am",&directive_list,&instruction_list,&instruction_counter,&directive_counter) < 0)
    {
        fprintf(stderr,"MACRO ERROR\n");
    }
    else if(memory(instruction_list,instruction_counter,&instruction_data_list) < 0)
    {
        fprintf(stderr,"MEMORY ERROR\n");
    }
    else if(symbol_create(instruction_data_list,instruction_list,instruction_counter,directive_list,directive_counter,&symbol_list,&symbol_counter) < 0)
    {
        fprintf(stderr,"SYMBOL ERROR\n");
    }
    else if(machine_code_main("dude",symbol_list,symbol_counter,instruction_data_list,instruction_counter)== false)
    {
        fprintf(stderr,"MACHINE CODE ERROR\n");
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
    free(symbol_list);

    return 0;
}










// 0x3 0x4 0x5 0x6
//  1   2   3   4

// 0x13 0x14 0x14 0x15 0x16
//  1    2    3    4    5