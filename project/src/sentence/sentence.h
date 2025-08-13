#ifndef SENTENCE_H
#define SENTENCE_H
#include <stdio.h>
#include "main.h"

#define EXTERN_WORD_LENGTH 7
#define ENTRY_WORD_LEN 6
#define STRING_WORD_LEN 7
#define DATA_WORD_LEN 5 

typedef enum setnence_type_e
{
    SENTENCE_TYPE_UNKNOWN = -1,
    SENTENCE_TYPE_INSTRUCTION = 0,
    SENTENCE_TYPE_DIRECTIVE = 1,
    SENTENCE_TYPE_COMMENT = 2,
    SENTENCE_TYPE_EMPTY = 3,
    SENTENCE_ERROR = 4
} setnence_type;

typedef enum sign_e
{
    PLUS = 1,
    MINUS = -1
}sign;

typedef enum status_s
{
    FAILURE = -1,
    SUCCESS = 0
    
};

#define MAX_LINE 80




int is_directive_string(char *line);
int is_directive_data(char *line);
int is_directive_entry(char *line);
int is_directive_extern(char *line);
int is_directive(char *line);
int is_comment(char *line);
int is_empty(char *line);
int is_instruction(char *line);
int process_line(char *line);
int fill_directive_struct(char *line, directive ** list, int * directive_counter);
int fill_instruction_struct(char *line, instruction **list_instructions, int * instruction_counter);
int sentence_decider(char *file_am, instruction **list_instructions, directive **list_directives,
                    int * directive_counter, int * instruction_counter);
#endif 
