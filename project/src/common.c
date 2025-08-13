#include <stdio.h>
#include "main.h"

int get_command_index(char * command_name)
{
    int ret = -1;
    for(int i = 0; (i < COMMAND_AMONT) && (ret == -1); i++)
    {
        if(strncmp(command_name , commands[i].command_name , COMMAND_MAX_SIZE) == 0)
        {
            ret = i;
        }
    }

    return ret;
}


