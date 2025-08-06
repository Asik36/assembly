#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#define MEMORY_MAX_SIZE 8192
#define MEMORY_ADRESS_MAX_BITS 12
#define SYMBOL_MAX_SIZE 16
#define COMMAND_MAX_SIZE 5
#define ATTRIBUTE_NONE 0
#define ARE_MAX_BIT_SIZE 4
#define FUNCT_MAX_BIT_SIZE 4
#define OPERAND_MAX_BIT_SIZE 4
#define OPERAND_TYPE_MAX_BIT_SIZE 2
#define ADDRESSING_TYPES_AMONT 4
#define COMMAND_AMONT 16

enum attribute_access_type_e
{
    
    ATTRIBUTE_DATA = 1,
    ATTRIBUTE_STRING

};
enum attribute_data_type_e
{
    ATTRIBUTE_EXTERN = 1,
    ATTRIBUTE_ENTERY
};
typedef struct instruction_s
{
    uint16_t base_address : MEMORY_ADRESS_MAX_BITS;
    uint16_t offset       : MEMORY_ADRESS_MAX_BITS;
    uint8_t label           [SYMBOL_MAX_SIZE];
    uint8_t commnand        [COMMAND_MAX_SIZE];
    uint8_t src_operand        [SYMBOL_MAX_SIZE];
    uint8_t dest_operand        [SYMBOL_MAX_SIZE];

} instruction;

typedef struct directive_s
{
    uint8_t label       [SYMBOL_MAX_SIZE];   /* extern and entry shou*/
    uint8_t name        [SYMBOL_MAX_SIZE];
    uint32_t data_length; 
    void * data;                                   /* chars or ints*/
    enum attribute_access_type_e access_attribute; /* extern or entry */
    enum attribute_data_type_e data_attribute;   /* data or string */
} directive;

typedef struct symbol_s
{
    uint8_t  name           [SYMBOL_MAX_SIZE];
    uint16_t base_address : MEMORY_ADRESS_MAX_BITS;
    uint16_t offset       : MEMORY_ADRESS_MAX_BITS;
    uint32_t data_length; 
    void * data;
    /* value is calculated by adding the offset to the base address */
    enum attribute_access_type_e access_attribute; /* extern or entry */
    enum attribute_data_type_e data_attribute;   /* data or string */


} symbol;



typedef enum are
{
    ABSOLUTE    = 1 << 0, /*word is a value*/
    RELOCATABLE = 1 << 1, /*word value is taken from a memory address*/
    EXTERNAL    = 1 << 2  /*word value is taken from a diffrent file*/
} are_e;

typedef enum addressing_modes_e
{
    ADDRESSING_MODES_IMMEDIATE,  
    ADDRESSING_MODES_DIRECT,
    ADDRESSING_MODES_INDEX,
    ADDRESSING_MODES_REGISTER_DIRECT
}addressing_modes;


typedef int16_t value_content;          /*Value data such as number or char */
typedef uint16_t opcode_content;        /*Opcode of command*/
typedef uint16_t offset_content;        /*Size of data in memory*/
typedef uint16_t base_address_content;  /*Data location in memory*/
/*Info about the operands of the command*/
typedef struct operand_content_s
{
    uint16_t funct                  : FUNCT_MAX_BIT_SIZE;
    uint16_t src_operand               : OPERAND_MAX_BIT_SIZE;         /*first operand*/
    addressing_modes src_operand_type  : OPERAND_TYPE_MAX_BIT_SIZE;    /*first operand's type*/
    uint16_t dest_operand               : OPERAND_MAX_BIT_SIZE;         /*second operand*/
    addressing_modes dest_operand_type  : OPERAND_TYPE_MAX_BIT_SIZE;    /*second operand's type*/
    
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
    are_e are_attribute : ARE_MAX_BIT_SIZE; /*ARE flag*/
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
    uint8_t command_name[COMMAND_MAX_SIZE];
    bool src_operand_types[ADDRESSING_TYPES_AMONT];
    bool dest_operand_types[ADDRESSING_TYPES_AMONT];
}command;

const command commands[COMMAND_AMONT] =
{
    {.opcode = OPCODE_MOV,.funct = 0,.command_name = "mov",.src_operand_types = {1,1,1,1} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_CMP,.funct = 0,.command_name = "cmp",.src_operand_types = {1,1,1,1} ,.dest_operand_types = {1,1,1,1}},
    {.opcode = OPCODE_MATH_OPERATION,.funct = 10,.command_name = "add",.src_operand_types = {1,1,1,1} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_MATH_OPERATION,.funct = 11,.command_name = "add",.src_operand_types = {1,1,1,1} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_LEA,.funct = 0,.command_name = "lea",.src_operand_types = {0,1,1,0} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_CHANGE_VALUE,.funct = 10,.command_name = "clr",.src_operand_types = {0} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_CHANGE_VALUE,.funct = 11,.command_name = "not",.src_operand_types = {0} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_CHANGE_VALUE,.funct = 12,.command_name = "inc",.src_operand_types = {0} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_CHANGE_VALUE,.funct = 13,.command_name = "dec",.src_operand_types = {0} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_JUMPS,.funct = 0,.command_name = "jmp",.src_operand_types = {0} ,.dest_operand_types = {0,1,1,0}},
    {.opcode = OPCODE_JUMPS,.funct = 0,.command_name = "bne",.src_operand_types = {0} ,.dest_operand_types = {0,1,1,0}},
    {.opcode = OPCODE_JUMPS,.funct = 0,.command_name = "jsr",.src_operand_types = {0} ,.dest_operand_types = {0,1,1,0}},
    {.opcode = OPCODE_RED,.funct = 0,.command_name = "red",.src_operand_types = {0} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_PRN,.funct = 0,.command_name = "prn",.src_operand_types = {0} ,.dest_operand_types = {0,1,1,1}},
    {.opcode = OPCODE_RTS,.funct = 0,.command_name = "rts",.src_operand_types = {0} ,.dest_operand_types = {0}},
    {.opcode = OPCODE_STOP,.funct = 0,.command_name = "stop",.src_operand_types = {0} ,.dest_operand_types = {0}}

}



#endif 

// HELLO : mov #3, r1
/**
 * LABEL = HELLO
 * COMMAND = mov
 * op1 = #3
 * op2 = r1
 * 
 * 
 * 
 */



/* add hello , r2 */