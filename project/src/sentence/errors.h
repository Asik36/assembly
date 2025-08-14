#ifndef ERRORS_H
#define ERRORS_H

#define MAX_LABEL_SIZE 31

#define LABEL_PATTERN "^[[:space:]]*(([[:alpha:]][[:alnum:]]{0,30}):[[:space:]]*)?"
#define COMMAND_PATTERN "[a-z]{3,4}([[:space:]]+|$)"
#define OPPERANDS_PATTERN "(([^,[:space:]]+[[:space:]]*,[[:space:]]*[^,[:space:]]+)|([^,[:space:]]+))?[[:space:]]*$"

#define ENTRY_PATTERN "[[:space:]]*\\.entry[[:space:]]+[[:alpha:]][[:alnum:]]{0,30}[[:space:]]*$"

#define EXTERN_PATTERN "[[:space:]]*\\.extern[[:space:]]+[[:alpha:]][[:alnum:]]{0,30}[[:space:]]*$"

#define STRING_PATTERN "[[:space:]]*\\.string[[:space:]]+\"[^\"]+\"[[:space:]]*$"

#define DATA_PATTERN "[[:space:]]*\\.data[[:space:]]+[+-]?[[:digit:]]+([[:space:]]*,[[:space:]]*[+-]?[[:digit:]]+)*[[:space:]]*$"

#define INSTRUCTION_PATTERN_SIZE sizeof(LABEL_PATTERN) + sizeof(COMMAND_PATTERN) + sizeof(OPPERANDS_PATTERN)

#define DIRECTION_ENTRY_PATTERN_SIZE sizeof(LABEL_PATTERN) + sizeof(ENTRY_PATTERN)

#define DIRECTION_EXTERN_PATTERN_SIZE sizeof(LABEL_PATTERN) + sizeof(EXTERN_PATTERN)

#define DIRECTION_DATA_PATTERN_SIZE sizeof(LABEL_PATTERN) + sizeof(DATA_PATTERN)

#define DIRECTION_STRING_PATTERN_SIZE sizeof(LABEL_PATTERN) + sizeof(STRING_PATTERN)

typedef enum line_status
{
    ERR_COMPILING_PATTERN = 1,
    LINE_CORRECT = 0,
    LINE_ERR = -1
}line_status_e;

/**
 * @brief this function checks if a line is an instruction
 * 
 * @param line this is the line we check about
 * @return line_status_e we return LINE_CORRECT/ERR
 */
line_status_e instruction_error_check(char *line);

/**
 * @brief this function checks if a line is a direction entry 
 * 
 * @param line the line we check about
 * @return line_status_e we return LINE_CORRECT/ERR
 */

line_status_e direction_entry_error_check(char *line);

/**
 * @brief this function checks if a line is a direction extern 
 * 
 * @param line the line we check about
 * @return line_status_e we return LINE_CORRECT/ERR
 */

line_status_e direction_extern_error_check(char *line);

/**
 * @brief this function checks if a line is a direction data 
 * 
 * @param line the line we check about
 * @return line_status_e we return LINE_CORRECT/ERR
 */

line_status_e direction_data_error_check(char *line);

/**
 * @brief this function checks if a line is a direction string 
 * 
 * @param line the line we check about
 * @return line_status_e we return LINE_CORRECT/ERR
 */

line_status_e direction_string_error_check(char *line);




#endif