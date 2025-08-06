#ifndef MACHINE_CODE_H
#define MACHINE_CODE_H
#include <stdio.h>
#include <main.h>


typedef enum are
{
    Absolute    = 1 << 0,
    Relocatable = 1 << 1,
    External    = 1 << 2
} are_e;

typedef enum addressing_modes_e
{
    immediate,
    direct,
    index,
    register_direct
}addressing_modes;


typedef int16_t value_content;
typedef uint16_t opcode_content;
typedef uint16_t offset_content;
typedef uint16_t base_address_content;
typedef struct operand_content_s
{
    uint16_t funct                  : 4;
    uint16_t operand1               : 4;
    addressing_modes operand1_type  : 2; 
    uint16_t operand2               : 4; 
    addressing_modes operand2_type  : 2; 
    
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
    are_e are_attribute : 4;
    word_content content;
}word_data;



typedef struct machine_code_s
{
    word_data * words;

}machine_code;





#endif 