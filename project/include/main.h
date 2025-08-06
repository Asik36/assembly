#ifndef MAIN_H
#define MAIN_H
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#define MEMORY_MAX_SIZE 8192
#define MEMORY_ADRESS_MAX_BITS 12
#define SYMBOL_MAX_SIZE 16
#define COMMAND_MAX_SIZE 3
#define ATTRIBUTE_NONE 0
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
    uint8_t label   [SYMBOL_MAX_SIZE];
    uint8_t commnand[COMMAND_MAX_SIZE];
    uint8_t operand1[SYMBOL_MAX_SIZE];
    uint8_t operand2[SYMBOL_MAX_SIZE];

} instruction;

typedef struct directive_s
{
    uint16_t base_address : MEMORY_ADRESS_MAX_BITS;
    uint8_t label       [SYMBOL_MAX_SIZE];   /* extern and entry shou*/
    uint8_t name        [SYMBOL_MAX_SIZE];
    enum attribute_access_type_e access_attribute; /* extern or entry */
    enum attribute_data_type_e access_attribute;   /* data or string */
} directive;

typedef struct symbol_s
{
    uint8_t  name  [SYMBOL_MAX_SIZE];
    uint16_t base_address : MEMORY_ADRESS_MAX_BITS;
    uint16_t offset       : MEMORY_ADRESS_MAX_BITS;
    /* value is calculated by adding the offset to the base address */
    enum attribute_access_type_e access_attribute; /* extern or entry */
    enum attribute_data_type_e access_attribute;   /* data or string */
} symbol;



















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