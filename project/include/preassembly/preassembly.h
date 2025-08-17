#ifndef PREASSEMBLY_H
#define PREASSEMBLY_H
#include <stdio.h>
#include "common_structs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <search.h>
#define MAX_NAME_LEN 100
#define MACRO_WORD_LEN 5
#define MAX_LINE 80
#define MACRO_TABLE_SIZE 1000 
#define MACRO_FILE_NAME_EXTENSION ".am"

enum macro_def_e {NOT_MACRO,MACRO};

typedef struct macro_s
{
    char *name;
    char *content;
    int content_size;
} macro;


/**
 * @brief this function checks if the memory allocation was successful.
 * 
 * @param arr the pointer to the allocated memory
 * @return int retval - SUCCESS if allocation was successful, FAILURE otherwise
 */

 int check_memory_allocation(char *arr);

/**
 * @brief this function removes spaces from the start of a line.
 * 
 * @param line the line we want to fix
 * @return char* a pointer to the new line
 */

char *remove_spaces_from_start(char *line);

/**
 * @brief this function adds a line to the content of a macro.
 * 
 * @param macro_ptr this is a pointer to the macro we want to add the line to
 * @param line this is the line we want to add
 * @return int retval - SUCCESS if the line was added successfully, FAILURE otherwise
 */

status add_to_content(macro *macro_ptr, char *line);

/**
 * @brief this function puts into a hash table all the macros in the file
 * 
 * @param file_as_path the originial code file we recieve
 * @return status SUCCESS/FAILURE
 */

status list_macros(char *file_as_path);

/**
 * @brief this function prints the content of a macro to a file.
 * 
 * @param file_am this is the file we write into 
 * @param line this is the line we read from the original code file
 * @return status SUCCESS/FAILURE
 */

status print_macro(FILE *file_am, char *line);

/**
 * @brief Create a file am file
 * 
 * @param file_as the original code file
 * @param file_am the new file we create 
 * @return int retval - SUCCESS if the file was created successfully, FAILURE otherwise
 */

status create_file_am(char *file_as, char *file_am);

#endif 