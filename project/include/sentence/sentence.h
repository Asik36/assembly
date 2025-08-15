#ifndef SENTENCE_H
#define SENTENCE_H
#include <stdio.h>
#include "common_structs.h"
#include "errors.h"
#include "preassembly.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define EXTERN_WORD_LENGTH 7
#define ENTRY_WORD_LEN 6
#define STRING_WORD_LEN 7
#define DATA_WORD_LEN 5 

typedef enum setnence_type
{
    SENTENCE_TYPE_UNKNOWN = -1,
    SENTENCE_TYPE_INSTRUCTION = 0,
    SENTENCE_TYPE_DIRECTIVE = 1,
    SENTENCE_TYPE_COMMENT = 2,
    SENTENCE_TYPE_EMPTY = 3,
    SENTENCE_ERROR = 4
} setnence_type_e;

typedef enum sign_e
{
    PLUS = 1,
    MINUS = -1
}sign;

typedef enum module_status
{
    MODULE_STATUS_SUCCESS = 0,
    MODULE_STATUS_ERR_ALLOCATION_FAILED = -1,
    MODULE_STATUS_ERR_FILE = -2,
    MODULE_STATUS_ERR_LINE = -3
}module_status_e;

#define MAX_LINE 80

/**
 * @brief this function fills the data arr 
 * 
 * @param data_start pointer to where the data starts
 * @param list pointer to pointer to the directive list
 * @param directive_counter pointer to the directive counter
 * @param counter pointer to counter so we will know the length
 * @return status SUCCESS/FAILURE
 */

status fill_data_arr(char *data_start, directive **list, int* directive_counter, int *counter);

/**
 * @brief Get the length of string we are going to read
 * 
 * @param line the line we are reading from
 * @return int number that represents the length of the string
 */


int get_len_string(char *line);


/**
 * @brief Get the length of the data we are going to read
 * 
 * @param data_start a pointer to the start of the data
 * @return int number that represents the length
 */

int get_data_len(char *data_start);

/**
 * @brief this function gets a string of a number and parsing it to an int
 * 
 * @param number_ptr a pointer to the string that represents a number
 * @return int the number
 */

int parse_int_from_ptr(char *number_ptr);

/**
 * @brief we check if a line is a directive string
 * 
 * @param line the line we are checking about
 * @return sentence_type_e that represents the code of the kind of line
 */
setnence_type_e is_directive_string(char *line);

/**
 * @brief we check if a line is a directive data
 * 
 * @param line the line we are checking about
 * @return sentence_type_e that represents the code of the kind of line
 */

setnence_type_e is_directive_data(char *line);

/**
 * @brief we check if a line is a directive entry
 * 
 * @param line the line we are checking about
 * @return sentence_type_e that represents the code of the kind of line
 */

setnence_type_e is_directive_entry(char *line);

/**
 * @brief we check if a line is a directive extern
 * 
 * @param line the line we are checking about
 * @return sentence_type_e that represents the code of the kind of line
 */

setnence_type_e is_directive_extern(char *line);

/**
 * @brief this function checks if the line is a direction of any kind
 * 
 * @param line the line we check about
 * @return sentence_type_e that represents the code of the kind of line
 */

setnence_type_e is_directive(char *line);

/**
 * @brief we check if a line is a comment
 * 
 * @param line the line we are checking about
 * @return sentence_type_e that represents the code of the kind of line
 */

setnence_type_e is_comment(char *line);

/**
 * @brief we check if a line is an empty line
 * 
 * @param line the line we are checking about
 * @return sentence_type_e that represents the code of the kind of line
 */

setnence_type_e is_empty(char *line);

/**
 * @brief we check if a line is an instruction
 * 
 * @param line the line we are checking about
 * @return sentence_type_e that represents the code of the kind of line
 */

setnence_type_e is_instruction(char *line);

/**
 * @brief here we process what kine of line is it
 * 
 * @param line the line we check about
 * @return sentence_type_e that represents the code of the kind of line
 */

setnence_type_e process_line(char *line);

/**
 * @brief this function fills the directive struct fields
 * 
 * @param line this is the line which from we will get our content
 * @param list this is a pointer to pointer to the directive list(to allow safe reallocation)
 * @param directive_counter the counter counts how much directions we have found yet
 * @return status SUCCES/FAILURE
 */

status fill_directive_struct(char *line, directive ** list, int * directive_counter);

/**
 * @brief this function fills the instruction struct fields
 * 
 * @param line this is the line which from we will get our content
 * @param list this is a pointer to pointer to the instruction list(to allow safe reallocation)
 * @param directive_counter the counter counts how much instructions we have found yet
 * @return status SUCCES/FAILURE
 */

status fill_instruction_struct(char *line, instruction **list_instructions, int * instruction_counter);

/**
 * @brief this function responsible for going through the whole file and create 
 * two lists that deining our needed information about each kind of line
 * 
 * @param file_am the file we are reading from
 * @param list_instructions a pointer to a pointer to our instrucion list
 * (so reallocations can work safely)
 * @param list_directives a pointer to a pointer to our directives list
 * (so reallocations can work safely)
 * @param directive_counter counts how much directins we had  
 * @param instruction_counter counts how much instructions we had
 * @return status SUCCESS/FAILURE that depends on if there was a syntax error from whatever line we read
 */

status sentence_decider(char *file_am, instruction **list_instructions, directive **list_directives,
                    int * directive_counter, int * instruction_counter);

/**
 * @brief this function does the whole first two modules and handling errors
 * 
 * @param file_as the original assembly file
 * @param file_am the file am we create
 * @param directive_list a pointer to pointer of directive list
 * @param instruction_list a pointer to pointer of instruction list
 * @param instruction_counter 
 * @param directive_counter 
 * @return module_status_e repsents code error or success of the compilation
 */

module_status_e check_processing_modules(char *file_as, char* file_am, directive **directive_list, 
                instruction **instruction_list, int* instruction_counter, int * directive_counter);
#endif 
