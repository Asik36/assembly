
#include "sentence.h"

int parse_int_from_ptr(char *number_ptr)
{
    int sign_factor = PLUS;
    long numeric_value = 0;

    while(isspace(*number_ptr)) 
    {
        number_ptr++;
    }

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


setnence_type_e is_directive_string(char *line)
{
    setnence_type_e retval;
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

setnence_type_e is_directive_data(char *line)
{
    setnence_type_e retval;
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

setnence_type_e is_directive_entry(char *line)
{
    setnence_type_e retval;
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

setnence_type_e is_directive_extern(char *line)
{
    setnence_type_e retval;
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

setnence_type_e is_directive(char *line)
{
    setnence_type_e retval;
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

setnence_type_e is_comment(char *line)
{
    setnence_type_e retval = SENTENCE_TYPE_UNKNOWN;
    if(*line == ';')
    {
        retval = SENTENCE_TYPE_COMMENT;
    }
    return retval;
}

setnence_type_e is_empty(char *line)
{
    setnence_type_e retval = SENTENCE_TYPE_EMPTY;
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

setnence_type_e is_instruction(char *line)
{
    setnence_type_e retval;
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

        if (!isdigit(*data_start))
        {
            break;
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
        while(*string_start != '"' && *string_start != '\n' && *string_start != '\0')
        {
            string_start++;
        }
        string_start++;
        line = string_start;
        while(*string_start != '"' && *string_start != '\n' && *string_start != '\0')
        {
            string_start++;
        }
        int length = (int) (string_start - line);
        retval = length;
    }
    return retval;
}

setnence_type_e process_line(char *line)
{
    setnence_type_e retval;
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

status_e fill_data_arr(char *data_start, directive_t **list, int* directive_counter, int *counter)
{
    status_e retval = SUCCESS;
    int data_len = get_data_len(data_start);

    if (data_len <= 0)
    {
        retval = FAILURE;
    }
    else
    {
        int *tmp_ptr_to_data_arr = (int *)malloc(sizeof(int) * data_len);
        if (tmp_ptr_to_data_arr == NULL)
        {
            perror("Couldn't malloc\n");
            retval = FAILURE;
        }
        else
        {
            (*list)[*directive_counter].data = tmp_ptr_to_data_arr;

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
                (*counter)++;
                ((int *)(*list)[*directive_counter].data)[*counter - 1] = number;
                (*list)[*directive_counter].data_length = *counter;
                (*list)[*directive_counter].data_attribute = ATTRIBUTE_DATA;
                
                if (*data_start == '+' || *data_start == '-')
                {
                    data_start++;
                }
                while (isdigit(*data_start) || isspace(*data_start))
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
    return retval;
}

status_e fill_extern(char *line, directive_t **list, int *directive_counter)
{
    status_e retval = SUCCESS;
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
    else
    {
        retval = FAILURE;
    }
    return retval;
}

status_e fill_string(char *line,directive_t **list,int *directive_counter)
{
    status_e retval = SUCCESS;
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
    char *string_data = (char*) malloc((length_of_string+1)*sizeof(char));
    if(string_data == NULL)
    {
        perror("Couldn't malloc\n");
        retval = FAILURE;
    }
    else
    {

        char *ptr_to_String = string_data;
        char *start_of_string = strchr(line,'"');
        int idx = 0;
        line = start_of_string + 1;
        while(*line != '"' && *line != '\0')
        {
            ptr_to_String[idx] = *line;
            idx++;
            line++;
        }
        ptr_to_String[idx] = '\0';

        (*list)[*directive_counter].data = string_data; 
        (*list)[*directive_counter].data_length = length_of_string;

        (*list)[*directive_counter].data_attribute = ATTRIBUTE_STRING;
    }
    return retval;
}

status_e fill_entry(char *line, directive_t **list, int *directive_counter)
{
    status_e retval = SUCCESS;
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
    return retval;
}

status_e fill_data(char *line, directive_t **list, int *directive_counter)
{
    status_e retval = SUCCESS;
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
    data_start += DATA_WORD_LEN + 1;
    status_e is_data_filled = fill_data_arr(data_start,list,directive_counter,&counter);
    if(is_data_filled == FAILURE)
    {
        retval = FAILURE;
    }
    return retval;
}

status_e fill_directive_struct(char *line, directive_t ** list, int * directive_counter)
{
    status_e retval = SUCCESS;
    *list = (directive_t *)realloc(*list, (*directive_counter + 1) * sizeof(directive_t));
    if(*list == NULL)
    {
        retval = FAILURE;
    }
    if(retval == SUCCESS)
    {
        memset(*list+*directive_counter,0,sizeof(directive_t));
        /* we will have a case for each directive kind*/
        /* case of data directive */
        if(is_directive_data(line) == SENTENCE_TYPE_DIRECTIVE)
        {
            fill_data(line,list,directive_counter);
        }
        /* case of directive entry */
        else if(is_directive_entry(line) == SENTENCE_TYPE_DIRECTIVE && retval == SUCCESS)
        {
            fill_entry(line,list,directive_counter);
        }
        /* case of directive extern */
        else if(is_directive_extern(line) == SENTENCE_TYPE_DIRECTIVE && retval == SUCCESS) 
        {
            fill_extern(line,list,directive_counter);
        }
        /* case of directive string */
        else if(is_directive_string(line) == SENTENCE_TYPE_DIRECTIVE && retval == SUCCESS)
        {
            fill_string(line,list,directive_counter);
        }
        (*directive_counter)++;
    }
    return retval; 
}

status_e fill_instruction_struct(char *line, instruction_t **list_instructions, int * instruction_counter)
{
    status_e retval = SUCCESS;
    *list_instructions = (instruction_t *)realloc(*list_instructions, (*instruction_counter + 1) * sizeof(instruction_t));
    if(list_instructions == NULL)
    {
        retval = FAILURE;
    }
    else
    {  
        memset(*list_instructions+ *instruction_counter,0,sizeof(instruction_t));
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
    }
    return retval; 
}

status_e sentence_decider(char *file_am, instruction_t **list_instructions, directive_t **list_directives,
                    int * directive_counter, int * instruction_counter)
{
    status_e had_error = SUCCESS;
    setnence_type_e retval = SUCCESS;
    FILE *file = fopen(file_am, "r");
    if (file == NULL)
    {   
        perror("Failed to open file\n");
        had_error = FAILURE;  
    }
    if(had_error == SUCCESS)
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

module_status_e check_processing_modules(char *file_as, char* base_file_name, directive_t **directive_list, 
                instruction_t **instruction_list, int* instruction_counter, int * directive_counter)
{
    module_status_e retval = MODULE_STATUS_SUCCESS;
    status_e returned_val;
    
    char file_am[FILE_NAME_MAX_LEN] = {0}; 
    strncpy(file_am,base_file_name,FILE_NAME_MAX_LEN);
    strncat(file_am,MACRO_FILE_NAME_EXTENSION,strlen(MACRO_FILE_NAME_EXTENSION)+1);

    returned_val = list_macros(file_as);
    if(returned_val == SUCCESS)
    {
        returned_val = create_file_am(file_as,file_am);
        if(returned_val == FAILURE)
        {   
            retval = MODULE_STATUS_ERR_ALLOCATION_FAILED;
            fprintf(stderr,"error creating files code error: %d\n",retval);
        }
    }
    else
    {
        retval = MODULE_STATUS_ERR_ALLOCATION_FAILED;
        fprintf(stderr,"error in allocating arrays in list macros\n");
    }
    if(retval == MODULE_STATUS_SUCCESS)
    {
        returned_val = sentence_decider(file_am,instruction_list,directive_list,directive_counter,instruction_counter);
        if(returned_val == FAILURE)
        {
            retval = MODULE_STATUS_ERR_LINE;
            fprintf(stderr,"error in one of the lines\n");
        }
    }
    return retval;
}