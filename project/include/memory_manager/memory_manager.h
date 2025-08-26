#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H
#include <stdio.h>
#include <stdlib.h>
#include "common_structs.h"
#define ADDRESSING_MODES_NONE -1 
#define SIGN_ADDRESSING_MODES_IMMEDIATE '#'
#define MSG_MEM_ERR_INVALID_SRC_ADDR_MODE   "ERROR: Invalid addressing mode for source operand\n"
#define MSG_MEM_ERR_INVALID_DEST_ADDR_MODE  "ERROR: Invalid addressing mode for destination operand\n"
#define MSG_MEM_ERR_INVALID_COMMAND         "ERROR: Invalid command\n"
#define MSG_MEM_ERR_UNDEFINED               "ERROR: Undefined\n"

typedef enum memory_status_e
{
    MEMORY_STATUS_ERR_INVALID_COMMAND = -3,
    MEMORY_STATUS_ERR_INVALID_SOURCE_ADRESSING_MODE,
    MEMORY_STATUS_ERR_INVALID_DESTINATION_ADDRESSING_MODE,
    MEMORY_STATUS_SUCCESS = 0,

} memory_status;



/** 
 * @brief main function that goes over instruction table checks if command is valid and assign memory address
 * @param instruction_line_table list of instruction_t in string form
 * @param n number of instructions
 * @param instruction_info_table list of instruction_t data
 * @return SUCCESS if all lines are valid otherwise if one line is invalid returns FAILURE
 */
memory_status memory(instruction_t *instruction_line_table,int n, instruction_data_t ** instruction_info_table);

/**
 * @brief extract info from instruction line into instruction data
 * @param instruction_line instruction_t strings
 * @param instruction_info instruction_t data
 * @return SUCCESS if line is valid otherwise FAILURE
 */
memory_status memory_instruction_assign(instruction_t *instruction_line, instruction_data_t *instruction_info);

/** @brief get address of the next instruction
 *  @param instruction_size size of current instruction
 *  @return memory address 
 */
uint16_t memory_instruction_get_address(int instruction_size);

/** 
 * @brief get command index from the commands table
 * @param ins current instruction_t
 * @return command index , if command not found returns -1
 */
int memory_instruction_get_command_index(instruction_t *ins);

/** 
 * @brief gets info about both operands , if they dont exists they are kept empty
 * @param input_operand current operand as written by user
 * @param operand_info writes operand's data 
 */
void memory_operand_get_info(char * input_operand, operand_data_t *operand_info);

/** 
 * @brief get addressing mode of operand
 * @param op operand string
 * @return addressing mode type
 */
addressing_modes_e memory_operand_get_addressing_mode(char *op);

/** 
 * @brief get register number from operand
 * @param op operand string
 * @return returns register num , if not a register return -1 
 */
int memory_operand_get_register_index(char *op);

/** 
 * @brief get data of operand using the correct addressing modes
 * @param op_data operand data
 * @param op operand string
 * @param addr_mode addressing mode of op_data
 * @return returns data, if there is no data retutns 0
 */
int memory_operand_get_data(operand_data_t *op_data, char *op, addressing_modes_e addr_mode);

/** 
 * @brief gets size of instruction , or in other words the amont of words in instruction. it is canculated using addressing modes on each operand
 * @param ins_data current instruction_t
 * @return instruction size
 */
uint16_t memory_instruction_get_size(instruction_data_t *ins_data);

/** 
 * @brief gets name of the varible using addressing mode INDEX, the part of the string before the brakets []
 * @param op operand string
 * @param dest_str saves the varible name in here
 */
void memory_operand_addressing_mode_index_varible_name(char *op, char *dest_str);

/** 
 * @brief gets data using addressing mode INDEX, the part of the string inside the brakets []
 * @param op operand string
 * @return returns data or 0 if no data
 */
int memory_operand_addressing_mode_index_data(char *op);

/** 
 * @brief gets data using addressing mode IMMEDITATE , which is a value of an integer
 * @param op operand string
 * @return value of number
 */
int memory_addressing_mode_immediate_extract_data(char *op);

/** 
 * @brief Check if opcode of addressing mod IMMEDIATE - a integer value
 * @param op operand string
 * @return true if IMMEDITE otherwise false
 */
bool memory_is_addressing_mode_immediate(char *op);

/** 
 * @brief Check if opcode of addressing mod DIRECT - a varible name
 * @param op operand string
 * @return true if DIRECT otherwise false
 */
bool memory_is_addressing_mode_direct(char *op);

/** 
 * @brief Check if opcode of addressing mod INDEX - an offset using register in a varible
 * @param op operand string
 * @return true if INDEX otherwise false
 */
bool memory_is_addressing_mode_index(char *op);

/** 
 * @brief Check if opcode of addressing mod REGISTER_DIRECT - a register
 * @param op operand string
 * @return true if REGISTER_DIRECT otherwise false
 */
bool memory_is_addressing_mode_register_direct(char *op);

/** 
 * @brief checks if instruction is valid
 * @param ins_data current instruction
 * @return SUCCSESS if instruction is valid otherwise FAILURE
 */
memory_status memory_instruction_validation(instruction_data_t *ins_data);

/** 
 * @brief gets the string between brackets []
 * @param src source string with brackets
 * @param dest string inside the brackets
 * @param dest_size max size of string inside the brackets
 * @return true if found string inside the brackets , if string not found return false 
 */
bool get_substring_between_brackets(const char *src, char *dest, size_t dest_size);

/**
 * @brief gets the extra size of instruction based on addressing mode 
 * 
 * @param operand instruction's operand
 * @return size form operand's addressing mode
 */
int memory_addressing_mode_instruction_size(operand_data_t * operand);


/**
 * @brief prints error messeges 
 * @param m_status error code
 */
void memory_error_handle(memory_status m_status);


/**
 * @brief check if there should be an operand
 * 
 * @param addressing_modes addressing mode available for opeand
 * @return true if at least one addressing_modes mode is true
 * @return false if all addressing_modes are false
 */
bool memory_have_operand(const bool addressing_modes[ADDRESSING_TYPES_AMOUNT]);
#endif
