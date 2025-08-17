#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#define MEMORY_MAX_SIZE 8192
#define MEMORY_ADRESS_MAX_BITS 12
#define SYMBOL_MAX_SIZE 16
#define COMMAND_MAX_SIZE 5
#define ARE_MAX_BIT_SIZE 4
#define FUNCT_MAX_BIT_SIZE 4
#define REGISTER_MAX_BIT_SIZE 4
#define OPERAND_TYPE_MAX_BIT_SIZE 2
#define ADDRESSING_TYPES_AMOUNT 4
#define FILE_NAME_MAX_LEN 256
#define COMMAND_AMONT 16
#define OPERAND_AMONT 16
#define ATTRIBUTE_NONE 0
#define is_error(s) ((s) < 0)


typedef enum status_e
{
    FAILURE = -1,
    SUCCESS = 0
}status;

typedef enum main_status_e
{
    MAIN_STATUS_ERR_MACHINE_CODE = -7,
    MAIN_STATUS_ERR_SYMBOL,
    MAIN_STATUS_ERR_MEMORY ,
    MAIN_STATUS_ERR_SENTENCE ,
    MAIN_STATUS_ERR_MACRO ,
    MAIN_STATUS_ERR_OUT_FILES,
    MAIN_STATUS_ERR_IN_FILES,
    MAIN_STATUS_SUCCESS = 0
}main_status;

enum attribute_data_type_e
{

    ATTRIBUTE_DATA_NONE = 0,
    ATTRIBUTE_DATA = 1,
    ATTRIBUTE_STRING,
    ATTRIBUTE_CODE

};
enum attribute_access_type_e
{
    ATTRIBUTE_ACCESS_NONE = 0,
    ATTRIBUTE_EXTERN,
    ATTRIBUTE_ENTERY
};

typedef enum are_e
{
    ABSOLUTE    = 1 << 2, /*word is a value*/
    RELOCATABLE = 1 << 1, /*word value is taken from a memory address*/
    EXTERNAL    = 1 << 0  /*word value is taken from a diffrent file*/
} are;

typedef enum addressing_modes_e
{
    ADDRESSING_MODES_NONE = -1,
    ADDRESSING_MODES_IMMEDIATE,
    ADDRESSING_MODES_DIRECT,
    ADDRESSING_MODES_INDEX,
    ADDRESSING_MODES_REGISTER_DIRECT
}addressing_modes;

typedef struct instruction_s
{

    char label          [SYMBOL_MAX_SIZE];
    char commnand       [COMMAND_MAX_SIZE];
    char src_operand    [SYMBOL_MAX_SIZE];
    char dest_operand   [SYMBOL_MAX_SIZE];

} instruction;



typedef struct operand_data_s
{
    addressing_modes addressing_mode;
    char varible_name[SYMBOL_MAX_SIZE];
    int operand_data;       /* Number value OR register index */

}operand_data;


typedef struct directive_s
{
    char variable_name       [SYMBOL_MAX_SIZE];   /* extern and entry shou*/
    uint32_t data_length;
    void * data;                                   /* chars or ints*/
    enum attribute_access_type_e access_attribute; /* extern or entry */
    enum attribute_data_type_e data_attribute;   /* data or string */
} directive;



typedef struct instruction_data_s
{
    uint16_t address : MEMORY_ADRESS_MAX_BITS;
    uint16_t size : MEMORY_ADRESS_MAX_BITS;
    int command_index;
    operand_data dest_operand_data;
    operand_data src_operand_data;

}instruction_data;



typedef struct symbol_s
{
    uint16_t address : MEMORY_ADRESS_MAX_BITS;
    uint16_t size    : MEMORY_ADRESS_MAX_BITS;

    char  name    [SYMBOL_MAX_SIZE];
    void * data; /*if data type int / if string type char*/
    enum attribute_access_type_e access_attribute; /* extern or entry */
    enum attribute_data_type_e data_attribute;   /* data or string */

} symbol;





typedef int16_t value_content;          /*Value data such as number or char */
typedef uint16_t opcode_content;        /*Opcode of command*/
typedef uint16_t offset_content;        /*Size of data in memory*/
typedef uint16_t base_address_content;  /*Data location in memory*/
/*Info about the operands of the command*/
typedef struct operand_content_s
{
    uint16_t funct             ;//          : FUNCT_MAX_BIT_SIZE;
    uint16_t src_register       ;//         : REGISTER_MAX_BIT_SIZE;         /*first operand*/
    uint16_t src_operand_type   ;// : OPERAND_TYPE_MAX_BIT_SIZE;    /*first operand's type*/
    uint16_t dest_register      ;//         : REGISTER_MAX_BIT_SIZE;         /*second operand*/
    uint16_t dest_operand_type ;//  : OPERAND_TYPE_MAX_BIT_SIZE;    /*second operand's type*/

}operand_content;

typedef enum opcode_types_e
{
    OPCODE_MOV              = 1 << 0,
    OPCODE_CMP              = 1 << 1,
    OPCODE_MATH_OPERATION   = 1 << 2,
    OPCODE_LEA              = 1 << 4,
    OPCODE_CHANGE_VALUE     = 1 << 5,
    OPCODE_JUMPS            = 1 << 9,
    OPCODE_RED              = 1 << 12,
    OPCODE_PRN              = 1 << 13,
    OPCODE_RTS              = 1 << 14,
    OPCODE_STOP             = 1 << 15

}opcode_types;



typedef union word_content_u
{
    opcode_content opcode;
    operand_content operand;
    base_address_content data_address;
    offset_content offset;
    value_content value;
}word_content;

typedef struct word_data_s
{
    are are_attribute : ARE_MAX_BIT_SIZE; /*ARE flag*/
    word_content content;                   /*can be : opcode/operand/adress/offset/value*/
}word_data;



typedef struct machine_code_s
{
    uint32_t word_count; /*number of words*/
    word_data * words; /*Array of words*/

}machine_code;



typedef struct command_s
{
    opcode_types opcode;
    uint16_t funct;
    char command_name[COMMAND_MAX_SIZE];
    bool src_operand_types[ADDRESSING_TYPES_AMOUNT];
    bool dest_operand_types[ADDRESSING_TYPES_AMOUNT];
}command;

static const command commands[COMMAND_AMONT] =
{
    {.opcode = OPCODE_MOV,.funct = 0,.command_name = "mov",.src_operand_types = {1,1,1,1} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_CMP,.funct = 0,.command_name = "cmp",.src_operand_types = {1,1,1,1} ,.dest_operand_types = {1,1,1,1}},
    {.opcode = OPCODE_MATH_OPERATION,.funct = 10,.command_name = "add",.src_operand_types = {1,1,1,1} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_MATH_OPERATION,.funct = 11,.command_name = "sub",.src_operand_types = {1,1,1,1} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_LEA,.funct = 0,.command_name = "lea",.src_operand_types = {0,1,1,0} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_CHANGE_VALUE,.funct = 10,.command_name = "clr",.src_operand_types = {0} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_CHANGE_VALUE,.funct = 11,.command_name = "not",.src_operand_types = {0} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_CHANGE_VALUE,.funct = 12,.command_name = "inc",.src_operand_types = {0} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_CHANGE_VALUE,.funct = 13,.command_name = "dec",.src_operand_types = {0} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_JUMPS,.funct = 0,.command_name = "jmp",.src_operand_types = {0} ,     .dest_operand_types = {0,1,1,0}},
    {.opcode = OPCODE_JUMPS,.funct = 0,.command_name = "bne",.src_operand_types = {0} ,.dest_operand_types = {0,1,1,0}},
    {.opcode = OPCODE_JUMPS,.funct = 0,.command_name = "jsr",.src_operand_types = {0} ,.dest_operand_types = {0,1,1,0}},
    {.opcode = OPCODE_RED,.funct = 0,.command_name = "red",.src_operand_types = {0} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_PRN,.funct = 0,.command_name = "prn",.src_operand_types = {0} ,.dest_operand_types = {1,1,1,1}},
    {.opcode = OPCODE_RTS,.funct = 0,.command_name = "rts",.src_operand_types = {0} ,.dest_operand_types = {0}},
    {.opcode = OPCODE_STOP,.funct = 0,.command_name = "stop",.src_operand_types = {0} ,.dest_operand_types = {0}}

};
main_status assembly(char * file_name);

void free_lists(int directive_counter, directive *directive_list, instruction *instruction_list, instruction_data *instruction_data_list, symbol *symbol_list);
status get_base_file_name(char * input_file , char * base_file_name);
void main_error_check(main_status m_status);

int get_command_index(char * command_name);


#endif
