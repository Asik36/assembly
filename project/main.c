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

    check_processing_modules("test.as", "test.am", &directive_list, &instruction_list, &instruction_counter, &directive_counter);
    // else if(memory(instruction_list,instruction_counter,&instruction_data_list) == FAILURE)
    // {
    //     fprintf(stderr,"MEMORY ERROR\n");
    // }

    printf("instuction counter = %d\n", instruction_counter);
    printf("direction counter = %d\n",directive_counter);


    
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