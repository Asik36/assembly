#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <search.h>
#include "preassembly.h"

typedef enum status_e
{
    FAILURE = -1,
    SUCCESS = 0
};


int check_memory_allocation(char *arr)
{
    int retval = SUCCESS;
    if(arr == NULL)
    {
        retval = FAILURE;
    }
    return retval;
}

char *remove_spaces_from_start(char *line)
{
    char *start = line;
    while (isspace(*start)) 
    {
        start++;
    }
    int  new_len = strlen(start);
    char *fixed = malloc(new_len + 1);
    if (fixed == NULL) 
    {
        printf("Failed to allocate memory for fixed line\n");
        return NULL;
    }
    strcpy(fixed, start);
    return fixed;
}


int add_to_content(macro *macro_ptr, char *line)
{
    int retval = SUCCESS;
    int size_of_line_read = strlen(line);
    int current_content_len = macro_ptr->content ? strlen(macro_ptr->content) : 0;
    int new_size = current_content_len + size_of_line_read + 1;
    char *tmp = realloc(macro_ptr->content, new_size);
    if (tmp == NULL)
    {
        retval = FAILURE;
    }
    else
    {
        macro_ptr->content = tmp;
        if (current_content_len == 0)
        {
            macro_ptr->content[0] = '\0';
        }
        char *content_line = remove_spaces_from_start(line);
        strcat(macro_ptr->content, content_line);
        free(content_line);
        macro_ptr->content_size = new_size;
    }
    return retval;
}

int list_macros(char *file_as_path)
{
    int retval = SUCCESS;
    FILE* file_as = fopen(file_as_path,"r");
    if(!file_as)
    {
        perror("Couldn't open file\n");
        retval = FAILURE;
    }
    if(retval == SUCCESS)
    {
        hcreate(MACRO_TABLE_SIZE);
        char line[MAX_LINE] = {0};
        while(fgets(line,MAX_LINE,file_as)!= NULL && retval == SUCCESS)
        {
            char *pointer_to_macro = strstr(line, "macro");
            if(pointer_to_macro != NULL)
            {

                if(strncmp(pointer_to_macro, "macro ", MACRO_WORD_LEN + 1) != 0)
                {
                    continue;
                }
                macro *new_macro = malloc(sizeof(macro));
                if (!new_macro)
                {
                    printf("allocation faild\n");
                    break;
                    
                }
                new_macro->name = malloc(MAX_NAME_LEN);
                retval = check_memory_allocation(new_macro->name);
                if(retval == FAILURE)
                {
                    free(new_macro);
                    printf("allocation faild\n");
                    break;
                }
                new_macro->content = malloc(1); // 1 for '\0'
                retval = check_memory_allocation(new_macro->content);
                if(retval == FAILURE)
                {
                    free(new_macro->content);
                    free(new_macro->name);
                    free(new_macro);
                    printf("allocation faild\n");
                    break;
                }
                new_macro->content[0] = '\0';
                new_macro->content_size = 1;
                pointer_to_macro += MACRO_WORD_LEN + 1;
                char *name_ptr = strtok(pointer_to_macro," \t\n");
                strncpy(new_macro->name, name_ptr, MAX_NAME_LEN - 1);
                new_macro->name[MAX_NAME_LEN - 1] = '\0';

                while(fgets(line,MAX_LINE,file_as) != NULL)
                {
                    pointer_to_macro = strstr(line, "endm");
                    if(pointer_to_macro == NULL)
                    {
                        if((add_to_content(new_macro, line)) == FAILURE)
                        {
                            free(new_macro->content);
                            free(new_macro->name);
                            free(new_macro);
                            retval = FAILURE;
                            printf("failed to add content of the macro\n");
                            break;
                        }
                    }
                    else
                    {
                        break;
                    }
                }
                ENTRY item;
                ENTRY *found;
                item.key = new_macro->name;
                item.data = (void*)new_macro;
                if(hsearch(item, ENTER) == NULL)
                {
                    perror("Error adding macro to hash table\n");
                    free(new_macro->content);
                    free(new_macro->name);
                    free(new_macro);
                    retval = FAILURE;
                    break;
                }
            }
        }
    }
    fclose(file_as);
    return retval;
}

int print_macro(FILE *file_am, char *line)
{
    int retval = SUCCESS;
    char *word = strtok(line, " \t\n\0");
    while(word)
    {
        ENTRY item;
        ENTRY *found;
        item.key = word;
        found = hsearch(item, FIND);
        if(found != NULL)
        {
            macro *macro_ptr = (macro *)(found->data);
            macro_ptr->content[strlen(macro_ptr->content)] = '\0';
            fprintf(file_am, "%s", macro_ptr->content);

        }
        else
        {
            fprintf(file_am, "%s ", word);
        }
        word = strtok(NULL, " \t\n\0");
    
    }
    fprintf(file_am, "\n");
    return retval;
}


int create_file_am(char *file_as, char *file_am)
{
    int retval = SUCCESS;
    FILE* original_code = fopen(file_as,"r");
    if(!original_code)
    {
        perror("Couldn't open file assembly\n");
        retval = FAILURE;
    }
    if(retval == SUCCESS)
    {
        FILE  *printed_macros_file = fopen(file_am, "w");
        if(!printed_macros_file)
        {
            perror("Couldn't open file am\n");
            retval = FAILURE;
            fclose(original_code);
        }
        if(retval == SUCCESS)
        {
            int is_macro_def = NOT_MACRO;
            char line[MAX_LINE];
            while(fgets(line,MAX_LINE,original_code) != NULL)
            {
                if(is_macro_def == NOT_MACRO)
                {
                    char *macro_start = strstr(line, "macro");
                    if(macro_start != NULL)
                    {
                        is_macro_def = MACRO;
                    }
                    else
                    {
                        print_macro(printed_macros_file,line);
                    }
                }
                else
                {
                    char *end_macro = strstr(line, "endm");
                    if(end_macro != NULL)
                    {
                        is_macro_def = NOT_MACRO;
                    }
                }
            }
            fclose(printed_macros_file);
        }
    }
    fclose(original_code);
    hdestroy();
    return retval;
}