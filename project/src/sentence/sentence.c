#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "sentence.h"
#include "errors.h"



int parse_int_from_ptr(char *number_ptr)
{
    int sign_factor = PLUS;
    long numeric_value = 0;

    while(isspace(*number_ptr)) number_ptr++;

    if(*number_ptr == '+' || *number_ptr == '-')
    {
        if(*number_ptr == '-') 
        {
            sign_factor = MINUS;
        }
        number_ptr++;
    }

    while(isdigit(*number_ptr))
    {
        numeric_value = numeric_value * 10 + (*number_ptr - '0');
        number_ptr++;
    }

    return (int)(sign_factor * numeric_value);
}


int is_directive_string(char *line)
{
    int retval;
    line_status_e line_status = direction_string_error_check(line);
    if(line_status == LINE_CORRECT)
    {
        retval = SENTENCE_TYPE_DIRECTIVE;
    }
    else
    {
        retval = SENTENCE_TYPE_UNKNOWN;
    }
    return retval;
}

int is_directive_data(char *line)
{
    int retval;
    line_status_e line_status = direction_data_error_check(line);
    if(line_status == LINE_CORRECT)
    {
        retval = SENTENCE_TYPE_DIRECTIVE;
    }
    else
    {
        retval = SENTENCE_TYPE_UNKNOWN;
    }
    return retval;
}

int is_directive_entry(char *line)
{
    int retval;
    line_status_e line_status = direction_entry_error_check(line);
    if(line_status == LINE_CORRECT)
    {
        retval = SENTENCE_TYPE_DIRECTIVE;
    }
    else
    {
        retval = SENTENCE_TYPE_UNKNOWN;
    }
    return retval;
}

int is_directive_extern(char *line)
{
    int retval;
    line_status_e line_status = direction_extern_error_check(line);
    if(line_status == LINE_CORRECT)
    {
        retval = SENTENCE_TYPE_DIRECTIVE;
    }
    else
    {
        retval = SENTENCE_TYPE_UNKNOWN;
    }
    return retval;
}

int is_directive(char *line)
{
    int retval;
    if(is_directive_string(line) == SENTENCE_TYPE_DIRECTIVE)
    {
        retval = SENTENCE_TYPE_DIRECTIVE;
    }
    else if(is_directive_data(line) == SENTENCE_TYPE_DIRECTIVE)
    {
        retval = SENTENCE_TYPE_DIRECTIVE;
    }
    else if(is_directive_entry(line) == SENTENCE_TYPE_DIRECTIVE)
    {
        retval = SENTENCE_TYPE_DIRECTIVE;
    }
    else if (is_directive_extern(line) == SENTENCE_TYPE_DIRECTIVE)
    {
        retval = SENTENCE_TYPE_DIRECTIVE;
    }
    else
    {
        retval = SENTENCE_TYPE_UNKNOWN;
    }
    return retval;
}

int is_comment(char *line)
{
    int retval = -1;
    if(*line == ';')
    {
        retval = SENTENCE_TYPE_COMMENT;
    }
    return retval;
}

int is_empty(char *line)
{
    int retval = SENTENCE_TYPE_EMPTY;
    while(*line != '\0' && *line != '\n' &&retval == SENTENCE_TYPE_EMPTY)
    {
        if(!isspace(*line))
        {
            retval = SENTENCE_TYPE_UNKNOWN;    
        }
        line++;
    }
    return retval;
}

int is_instruction(char *line)
{
    int retval;
    line_status_e line_type;
    line_type = instruction_error_check(line);
    if(line_type == LINE_CORRECT)
    {
        retval = SENTENCE_TYPE_INSTRUCTION;
    }
    else
    {
        retval = SENTENCE_TYPE_UNKNOWN;
    }
    return retval;
}

int get_data_len(char *data_start)
{
    int counter = 0;
    while(*data_start != '\0' && *data_start != '\n')
    {
        while(isspace(*data_start)) 
        { 
            data_start++; 
        }
        if(*data_start == '\0' || *data_start == '\n')
        {
            break;
        } 
        if (*data_start == '+' || *data_start == '-') 
        {
            data_start++; 
        }
        while (isdigit(*data_start)) 
        { 
            data_start++; 
        }
        counter++;
        while (isspace(*data_start)) 
        {
            data_start++; 
        }
        if (*data_start == ',')
        {
            data_start++;
            while (isspace(*data_start)) 
            { 
                data_start++; 
            }
        }  
    }
    return counter;
}

int get_len_string(char *line)
{
    int retval = FAILURE;
    char *string_start = strstr(line,".string");
    if(string_start)
    {
        string_start += STRING_WORD_LEN;
        line = string_start;
        while(*string_start != '"')
        {
            string_start++;
        }
        string_start++;
        line = string_start;
        while(*string_start != '"')
        {
            string_start++;
        }
        int length = (int) (string_start - line);
        retval = length;
    }
    return retval;
}

int process_line(char *line)
{
    int retval;
    if(is_empty(line) == SENTENCE_TYPE_EMPTY)
    {
        retval = SENTENCE_TYPE_EMPTY;
    }
    else if(is_comment(line) == SENTENCE_TYPE_COMMENT)
    {
        retval = SENTENCE_TYPE_COMMENT;
    }
    else if(is_instruction(line) == SENTENCE_TYPE_INSTRUCTION)
    {
        retval = SENTENCE_TYPE_INSTRUCTION;
    }
    else if(is_directive(line) == SENTENCE_TYPE_DIRECTIVE)
    {
        retval = SENTENCE_TYPE_DIRECTIVE;
    }
    else
    {
        retval = SENTENCE_ERROR;
        printf("ERROR: %s\n",line);
    }
    return retval;

}

int fill_directive_struct(char *line, directive ** list, int * directive_counter)
{
    int retval = SUCCESS;
    *list = (directive *)realloc(*list, (*directive_counter + 1) * sizeof(directive));
    if(*list == NULL)
    {
        retval = FAILURE;
    }
    if(retval == SUCCESS)
    {
        if(is_directive_data(line) == SENTENCE_TYPE_DIRECTIVE)
        {
            int counter = 0;

            char *is_label = strchr(line, ':'); 
            if(is_label)
            {
                int idx = 0;
                while(line != is_label)
                {
                    while(isspace(*line)) 
                    { 
                        line++; 
                    }
                    (*list)[*directive_counter].variable_name[idx++] = *line;
                    line++;
                }
                (*list)[*directive_counter].variable_name[idx] = '\0';
                line++;
            }

            char *data_start = strstr(line, ".data"); 
            if(data_start)
            {
                data_start += DATA_WORD_LEN; 
                int data_len = get_data_len(data_start);
                int *tmp_ptr_to_data_arr = (int *)malloc(sizeof(int) * data_len);
                if(tmp_ptr_to_data_arr == NULL)
                {
                    perror("Couldn't malloc\n");
                    retval = FAILURE;
                }
                while(*data_start != '\0' && *data_start != '\n')
                {
                    while(isspace(*data_start)) 
                    { 
                        data_start++; 
                    }
                    if(*data_start == '\0' || *data_start == '\n')
                    {
                        break;
                    } 
                    int number = parse_int_from_ptr(data_start);
                    counter++;
                    int *ptr_to_data_arr = (int *)(*list)[*directive_counter].data;
                    (*list)[*directive_counter].data = tmp_ptr_to_data_arr;
                    ptr_to_data_arr = (int *)(*list)[*directive_counter].data;
                    ptr_to_data_arr[counter - 1] = number;
                    (*list)[*directive_counter].data_length = counter;
                    (*list)[*directive_counter].data_attribute = ATTRIBUTE_DATA;
                    if (*data_start == '+' || *data_start == '-') 
                    {
                        data_start++; 
                    }
                    while (isdigit(*data_start)) 
                    { 
                        data_start++; 
                    }
                    while (isspace(*data_start)) 
                    {
                        data_start++; 
                    }
                    if (*data_start == ',')
                    {
                        data_start++;
                        while (isspace(*data_start)) 
                        { 
                            data_start++; 
                        }
                    }  
                }
            }
        }
        else if(is_directive_entry(line) == SENTENCE_TYPE_DIRECTIVE)
        {
            (*list)[*directive_counter].access_attribute = ATTRIBUTE_ENTERY;
            char *entry_declaration = strstr(line,".entry");
            entry_declaration += ENTRY_WORD_LEN;
            line = entry_declaration + 1;
            while(isspace(*line))
            {
                line++;
            }
            int idx = 0;
            while(!(isspace(*line)) && *line != '\n' && *line != '\0')
            {
                (*list)[*directive_counter].variable_name[idx] = *line;
                line++;
                idx++;
            }
            (*list)[*directive_counter].variable_name[idx] = '\0';
        }
        else if(is_directive_extern(line) == SENTENCE_TYPE_DIRECTIVE)
        {
            char *extern_start = strstr(line,".extern");
            if(extern_start != NULL)
            {
                extern_start += EXTERN_WORD_LENGTH;
                line = extern_start + 1;
                int idx= 0;
                while(isspace(*line))
                {
                    line++;
                }
                while(!(isspace(*line)) && *line != '\n' && *line != '\0')
                {
                    (*list)[*directive_counter].variable_name[idx] = *line;
                    line++;
                    idx++;
                }
                (*list)[*directive_counter].variable_name[idx] = '\0';
                (*list)[*directive_counter].access_attribute = ATTRIBUTE_EXTERN;
            }
        }
        else if(is_directive_string(line) == SENTENCE_TYPE_DIRECTIVE)
        {
            char *is_label = strchr(line, ':'); 
            if(is_label)
            {
                int idx = 0;
                while(line != is_label)
                {
                    while(isspace(*line)) 
                    { 
                        line++; 
                    }
                    (*list)[*directive_counter].variable_name[idx++] = *line;
                    line++;
                }
                (*list)[*directive_counter].variable_name[idx] = '\0';
                line++;
            }
            int length_of_string = get_len_string(line);
            char *string_data = (char*) malloc(length_of_string + 1);
            if(string_data == NULL)
            {
                perror("Couldn't malloc\n");
                retval = FAILURE;
            }
            else
            {
                (*list)[*directive_counter].data = string_data; 
                char *ptr_to_String = (char *)(*list)[*directive_counter].data;
                (*list)[*directive_counter].data = string_data;
                char *start_of_string = strchr(line,'"');
                int idx = 0;
                line = start_of_string + 1;
                while(*line != '"')
                {
                    ptr_to_String[idx] = *line;
                    idx++;
                    line++;
                }
                ptr_to_String[idx] = '\0';
                (*list)[*directive_counter].data_attribute = ATTRIBUTE_STRING;
            }
        }
        (*directive_counter)++;
    }
    return retval; 
}

int fill_instruction_struct(char *line, instruction **list_instructions, int * instruction_counter)
{
    *list_instructions = (instruction *)realloc(*list_instructions, (*instruction_counter + 1) * sizeof(instruction));
    char * is_label = strstr(line,":");
    if(is_label)
    {
        int label_len = (char *) is_label - (char*) line;
        strncpy((*list_instructions)[*instruction_counter].label,line,label_len);
        (*list_instructions)[*instruction_counter].label[label_len] = '\0';
        line = is_label + 1;
    }
    int index_arr = 0;
    while(isspace(*line)) line++;
    while(isalpha(*line))
    {
        (*list_instructions)[*instruction_counter].commnand[index_arr++] = *line++;
    }
    (*list_instructions)[*instruction_counter].commnand[index_arr] = '\0';
    index_arr = 0;
    while(isspace(*line)) line++;
    while(!isspace(*line) && *line != ',' && *line != '\n' && *line != '\0')
    {
        (*list_instructions)[*instruction_counter].src_operand[index_arr++] = *line++;
    }
    (*list_instructions)[*instruction_counter].src_operand[index_arr] = '\0';
    index_arr = 0;
    char * comma = strstr(line,",");
    if(comma)
    {
        line = comma + 1;
        while(isspace(*line)) line++;
        while(!isspace(*line) && *line != '\n' && *line != '\0')
        {
            (*list_instructions)[*instruction_counter].dest_operand[index_arr++] = *line++;
        }
        (*list_instructions)[*instruction_counter].dest_operand[index_arr] = '\0';
    }
    (*instruction_counter) ++;
    return 1; // FIX ME
}

int sentence_decider(char *file_am, instruction **list_instructions, directive **list_directives,
                    int * directive_counter, int * instruction_counter)
{
    int had_error = SUCCESS;
    int retval = SUCCESS;
    FILE *file = fopen(file_am, "r");
    if (file == NULL)
    {   
        perror("Failed to open file\n");
        retval = FAILURE;  
    }
    if(retval == SUCCESS)
    {
        char line[MAX_LINE];
        while (fgets(line, sizeof(line), file) != NULL)
        {
            retval = process_line(line);
            switch(retval)
            {
                case SENTENCE_TYPE_DIRECTIVE:
                    fill_directive_struct(line, list_directives, directive_counter);
                    break;
                case SENTENCE_TYPE_INSTRUCTION:
                    fill_instruction_struct(line, list_instructions, instruction_counter);
                    break;
                case SENTENCE_ERROR:
                    had_error = FAILURE;
                default:
                    break;
            }
            
        }
    }
    if(file_am != NULL)
    {
        fclose(file);
    }
    return had_error;
}