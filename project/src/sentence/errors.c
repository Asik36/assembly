#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <regex.h>
#include "errors.h"

line_status_e instruction_error_check(char *line)
{
    line_status_e retval;
    char pattern[INSTRUCTION_PATTERN_SIZE];
    pattern[0] = '\0';
    strcat(pattern,LABEL_PATTERN);
    strcat(pattern,COMMAND_PATTERN);
    strcat(pattern,OPPERANDS_PATTERN);
    regex_t regex;
    
    int ret_from_regcomp = regcomp(&regex,pattern,REG_EXTENDED);
    if(ret_from_regcomp != 0)
    {
        perror("reg not comipled correctly\n");
        retval = ERR_COMPILING_PATTERN;
    }
    else
    {
        int ret_check = regexec(&regex,line,0,NULL,0);
        if(ret_check != 0)
        {

            retval = LINE_ERR;
        }
        else
        {
            retval = LINE_CORRECT;
        }
    }
    return retval;
}

line_status_e direction_entry_error_check(char *line)
{
    line_status_e retval;
    char pattern[DIRECTION_ENTRY_PATTERN_SIZE];
    pattern[0] = '\0';
    strcat(pattern,LABEL_PATTERN);
    strcat(pattern,ENTRY_PATTERN);
    regex_t regex;
    
    int ret_from_regcomp = regcomp(&regex,pattern,REG_EXTENDED);
    if(ret_from_regcomp != 0)
    {
        perror("reg not comipled correctly\n");
        retval = ERR_COMPILING_PATTERN;
    }
    else
    {
        int ret_check = regexec(&regex,line,0,NULL,0);
        if(ret_check != 0)
        {
            retval = LINE_ERR;
        }
        else
        {
            retval = LINE_CORRECT;
        }
    }
    return retval;
}

line_status_e direction_extern_error_check(char *line)
{
    line_status_e retval;
    char pattern[DIRECTION_EXTERN_PATTERN_SIZE];
    pattern[0] = '\0';
    strcat(pattern,LABEL_PATTERN);
    strcat(pattern,EXTERN_PATTERN);
    regex_t regex;
    
    int ret_from_regcomp = regcomp(&regex,pattern,REG_EXTENDED);
    if(ret_from_regcomp != 0)
    {
        perror("reg not comipled correctly\n");
        retval = ERR_COMPILING_PATTERN;
    }
    else
    {
        int ret_check = regexec(&regex,line,0,NULL,0);
        if(ret_check != 0)
        {
            
            retval = LINE_ERR;
        }
        else
        {
            retval = LINE_CORRECT;
        }
    }
    return retval;
}

line_status_e direction_data_error_check(char *line)
{
    line_status_e retval;
    char pattern[DIRECTION_DATA_PATTERN_SIZE];
    pattern[0] = '\0';
    strcat(pattern,LABEL_PATTERN);
    strcat(pattern,DATA_PATTERN);
    regex_t regex;
    int ret_from_regcomp = regcomp(&regex,pattern,REG_EXTENDED);
    if(ret_from_regcomp != 0)
    {
        perror("reg not comipled correctly\n");
        retval = ERR_COMPILING_PATTERN;
    }
    else
    {
        int ret_check = regexec(&regex,line,0,NULL,0);
        if(ret_check != 0)
        {
            
            retval = LINE_ERR;
        }
        else
        {
            retval = LINE_CORRECT;
        }
    }
    return retval;
}

line_status_e direction_string_error_check(char *line)
{
    line_status_e retval;
    char pattern[DIRECTION_STRING_PATTERN_SIZE];
    pattern[0] = '\0';
    strcat(pattern,LABEL_PATTERN);
    strcat(pattern,STRING_PATTERN);
    regex_t regex;
    
    int ret_from_regcomp = regcomp(&regex,pattern,REG_EXTENDED);
    if(ret_from_regcomp != 0)
    {
        perror("reg not comipled correctly\n");
        retval = ERR_COMPILING_PATTERN;
    }
    else
    {
        int ret_check = regexec(&regex,line,0,NULL,0);
        if(ret_check != 0)
        {
            
            retval = LINE_ERR;
        }
        else
        {
            retval = LINE_CORRECT;
        }
    }
    return retval;
}
