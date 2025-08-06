#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#define MEMORY_MAX_SIZE 8192
#define MEMORY_ADRESS_MAX_BITS 12
#define SYMBOL_MAX_SIZE 16
#define COMMAND_MAX_SIZE 5
#define ATTRIBUTE_NONE 0
#define ARE_MAX_BIT_SIZE 4
#define FUNCT_MAX_BIT_SIZE 4
#define OPERAND_MAX_BIT_SIZE 4
#define OPERAND_TYPE_MAX_BIT_SIZE 2
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
    uint8_t operand1        [SYMBOL_MAX_SIZE];
    uint8_t operand2        [SYMBOL_MAX_SIZE];

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
    Absolute    = 1 << 0, /*word is a value*/
    Relocatable = 1 << 1, /*word value is taken from a memory address*/
    External    = 1 << 2  /*word value is taken from a diffrent file*/
} are_e;

typedef enum addressing_modes_e
{
    immediate,  
    direct,
    index,
    register_direct
}addressing_modes;


typedef int16_t value_content;          /*Value data such as number or char */
typedef uint16_t opcode_content;        /*Opcode of command*/
typedef uint16_t offset_content;        /*Size of data in memory*/
typedef uint16_t base_address_content;  /*Data location in memory*/
/*Info about the operands of the command*/
typedef struct operand_content_s
{
    uint16_t funct                  : FUNCT_MAX_BIT_SIZE;
    uint16_t operand1               : OPERAND_MAX_BIT_SIZE;         /*first operand*/
    addressing_modes operand1_type  : OPERAND_TYPE_MAX_BIT_SIZE;    /*first operand's type*/
    uint16_t operand2               : OPERAND_MAX_BIT_SIZE;         /*second operand*/
    addressing_modes operand2_type  : OPERAND_TYPE_MAX_BIT_SIZE;    /*second operand's type*/
    
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