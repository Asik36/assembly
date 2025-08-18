#include "memory_manager.h"
static const char *register_names[OPERAND_AMONT] =
    {
        "r1" , "r2" , "r3",
        "r4" , "r5" , "r6",
        "r7" , "r8" , "r9",
        "r10", "r11", "r12",
        "r13", "r14", "r15",
        "r16"
    };
memory_status memory(instruction *instruction_line_table, int n, instruction_data **instruction_info_table)
{

    memory_status retval = MEMORY_STATUS_SUCCESS;
    *instruction_info_table = (instruction_data *)calloc(1, n * sizeof(instruction_data));
    instruction_data *current_instruction_data;
    instruction *current_instruction_line;

    int i;
    for (i = 0; i < n && retval == MEMORY_STATUS_SUCCESS; i++)
    {
        current_instruction_line = instruction_line_table + i;
        current_instruction_data = *instruction_info_table + i;
        retval = memory_instruction_assign(current_instruction_line, current_instruction_data);
    }
    if (is_error(retval))
    {
        fprintf(stderr, " Error at instruction [%d]\n", i);
        memory_error_handle(retval);
    }

    return retval;
}
memory_status memory_instruction_assign(instruction *instruction_line, instruction_data *instruction_info)
{
    operand_data src_operand = {0}, dest_operand = {0};
    instruction_info->command_index = memory_instruction_get_command_index(instruction_line);
    command current_command = commands[instruction_info->command_index];

    memory_operand_get_info(instruction_line->src_operand, &src_operand);
    memory_operand_get_info(instruction_line->dest_operand, &dest_operand);

    bool is_src_operand = memory_have_operand(current_command.src_operand_types);
    bool is_dest_operand = memory_have_operand(current_command.dest_operand_types);

    /*if command has only one operand , put it operand as dest oprand*/
    if (is_src_operand == false && is_dest_operand == true)
    {
        instruction_info->dest_operand_data = src_operand;
        instruction_info->src_operand_data.addressing_mode = ADDRESSING_MODES_NONE;
    }
    else
    {
        instruction_info->src_operand_data = src_operand;
        instruction_info->dest_operand_data = dest_operand;
    }

    instruction_info->size = memory_instruction_get_size(instruction_info);
    instruction_info->address = memory_instruction_get_address(instruction_info->size);

    return memory_instruction_validation(instruction_info);
}
uint16_t memory_instruction_get_address(int instruction_size)
{
    static uint16_t address_count = START_ADDRESS;
    uint16_t new_address = address_count;
    address_count += instruction_size;
    return new_address;
}

int memory_instruction_get_command_index(instruction *ins)
{
    return get_command_index(ins->commnand);
}

void memory_operand_get_info(char *input_operand, operand_data *operand_info)
{
    operand_info->addressing_mode = memory_operand_get_addressing_mode(input_operand);
    if (operand_info->addressing_mode != ADDRESSING_MODES_NONE)
    {
        memory_operand_get_data(operand_info, input_operand, operand_info->addressing_mode);
    }
}

int memory_operand_get_register_index(char *op)
{
    int i;
    int index = -1;

    for (i = 0; i < OPERAND_AMONT && index == -1; i++)
    {

        if (strncmp(op, register_names[i], SYMBOL_MAX_SIZE) == 0)
        {
            index = i + 1;
        }
    }
    return index;
}

int memory_operand_get_data(operand_data *op_data, char *op, addressing_modes addr_mode)
{
    switch (addr_mode)
    {
    case ADDRESSING_MODES_IMMEDIATE:
        op_data->operand_data = memory_addressing_mode_immediate_extract_data(op);
        break;
    case ADDRESSING_MODES_DIRECT:
        strcpy(op_data->varible_name, op);
        break;
    case ADDRESSING_MODES_INDEX:
        op_data->operand_data = memory_operand_addressing_mode_index_data(op);
        memory_operand_addressing_mode_index_varible_name(op, op_data->varible_name);
        break;
    case ADDRESSING_MODES_REGISTER_DIRECT:
        op_data->operand_data = memory_operand_get_register_index(op);
        break;
    default:
        op_data->operand_data = 0;
        break;
    }
    return 0;
}
void memory_operand_addressing_mode_index_varible_name(char *op, char *dest_str)
{
    char *next = op;
    while (*next != '\0' && *next != '[')
    {
        next++;
    }
    strncpy(dest_str, op, next - op);
}

int memory_operand_addressing_mode_index_data(char *op)
{
    char reg[SYMBOL_MAX_SIZE] = {0};
    get_substring_between_brackets(op, reg, SYMBOL_MAX_SIZE);
    return memory_operand_get_register_index(reg);
}

uint16_t memory_instruction_get_size(instruction_data *ins_data)
{
    uint16_t size = 1; /*defualt size , each command is at least one word long*/
    operand_data *op_data_dest;
    operand_data *op_data_src;

    op_data_src = &ins_data->src_operand_data;
    op_data_dest = &ins_data->dest_operand_data;

    /* check if at least operand exists , if it does add a words based on addressing modes*/
    if (op_data_dest != NULL || op_data_src != NULL)
    {
        size += 1; /*extra word for funct and operands data*/
        size += memory_addressing_mode_instruction_size(op_data_src);
        size += memory_addressing_mode_instruction_size(op_data_dest);
    }
    return size;
}

int memory_addressing_mode_instruction_size(operand_data *operand)
{
    int size = 0;
    if (operand != NULL)
    {
        switch (operand->addressing_mode)
        {
        case ADDRESSING_MODES_IMMEDIATE:
            size += 1; /*extra word for storing value of nmuber*/
            break;
        case ADDRESSING_MODES_DIRECT:
            size += 2; /*extra two word for storing address and offset of a varible*/
            break;
        case ADDRESSING_MODES_INDEX:
            size += 2; /*extra two word for storing address and offset of a varible*/
            break;
        case ADDRESSING_MODES_REGISTER_DIRECT:
            size += 0; /*no extra words, registers are stored in the second word*/
            break;
        default:
            break;
        }
    }
    return size;
}

addressing_modes memory_operand_get_addressing_mode(char *op)
{
    addressing_modes mode;
    if (*op == '\0')
    {
        mode = ADDRESSING_MODES_NONE;
    }
    else if (memory_is_addressing_mode_immediate(op))
    {
        mode = ADDRESSING_MODES_IMMEDIATE;
    }
    else if (memory_is_addressing_mode_index(op))
    {
        mode = ADDRESSING_MODES_INDEX;
    }
    else if (memory_is_addressing_mode_register_direct(op))
    {
        mode = ADDRESSING_MODES_REGISTER_DIRECT;
    }
    else if (memory_is_addressing_mode_direct(op))
    {
        mode = ADDRESSING_MODES_DIRECT;
    }
    return mode;
}

bool memory_is_addressing_mode_immediate(char *op)
{
    bool is_immeditate = true;
    if (*op == SIGN_ADDRESSING_MODES_IMMEDIATE)
    {
        op++; /* skip # */
        while (*op != '\0' && is_immeditate)
        {
            if (!isdigit(*op) && !(*op == '-' && isdigit(*(op + 1))))
            {
                is_immeditate = false;
            }
            op++;
        }
    }
    else
    {
        is_immeditate = false;
    }
    return is_immeditate;
}
int memory_addressing_mode_immediate_extract_data(char *op)
{
    op++; /* skip # charcter */
    return (int)strtol(op, NULL, 10);
}
bool memory_is_addressing_mode_direct(char *op)
{
    bool is_direct = true;
    if (isdigit(*op))
    {
        is_direct = false;
    }
    op++;
    while (*op != '\0' && is_direct)
    {
        if (!isalnum(*op))
        {
            is_direct = false;
        }
        op++;
    }
    return is_direct;
}

memory_status memory_instruction_validation(instruction_data *ins_data)
{
    memory_status retval = MEMORY_STATUS_SUCCESS;
    int command_index = ins_data->command_index;
    int dest_addressing_mode = ins_data->dest_operand_data.addressing_mode;
    int src_addressing_mode = ins_data->src_operand_data.addressing_mode;
    if (command_index > COMMAND_AMONT || command_index < 0) 
    {
        retval = MEMORY_STATUS_ERR_INVALID_COMMAND;
    }
    else if (src_addressing_mode != ADDRESSING_MODES_NONE && commands[command_index].src_operand_types[src_addressing_mode] == false)
    {
        retval = MEMORY_STATUS_ERR_INVALID_SOURCE_ADRESSING_MODE;
    }
    else if (dest_addressing_mode != ADDRESSING_MODES_NONE && commands[command_index].dest_operand_types[dest_addressing_mode] == false)
    {
        retval = MEMORY_STATUS_ERR_INVALID_DESTINATION_ADDRESSING_MODE;
    }

    return retval;
}

bool memory_is_addressing_mode_index(char *op)
{
    bool is_direct = true;
    bool is_register = false;
    int i;
    char brackets_sub_str[SYMBOL_MAX_SIZE] = {0};
    if (get_substring_between_brackets(op, brackets_sub_str, SYMBOL_MAX_SIZE) == false)
    {
        is_direct = false;
    }
    /* if the first charcter is a number or if there is no varible name*/
    else if (isdigit(*op) || *op == '[')
    {
        is_direct = false;
    }

    for (i = 0; i < OPERAND_AMONT && is_register == false && is_direct == true; i++)
    {
        is_register = strncmp(register_names[i], brackets_sub_str, SYMBOL_MAX_SIZE) == 0;
    }

    return is_direct && is_register;
}

bool memory_is_addressing_mode_register_direct(char *op)
{
    return memory_operand_get_register_index(op) != -1;
}

bool get_substring_between_brackets(const char *src, char *dest, size_t dest_size)
{
    bool retval = false;
    const char *start = strchr(src, '[');
    const char *end = NULL;
    size_t len = 0;

    if (start)
    {
        end = strchr(start, ']');
        if (end)
        {
            len = end - start - 1;
            if (len >= dest_size)
            {
                len = dest_size - 1;
            }
            strncpy(dest, start + 1, len);
            dest[len] = '\0';
            retval = true;
        }
    }
    return retval;
}

bool memory_have_operand(const bool addressing_modes[ADDRESSING_TYPES_AMOUNT])
{
    bool operand_exits = false;
    for (int i = 0; i < ADDRESSING_TYPES_AMOUNT && operand_exits == false; i++)
    {
        operand_exits |= addressing_modes[i];
    }
    return operand_exits;
}

void memory_error_handle(memory_status m_status)
{
    switch (m_status)
    {
    case MEMORY_STATUS_ERR_INVALID_SOURCE_ADRESSING_MODE:
        fprintf(stderr, MSG_MEM_ERR_INVALID_SRC_ADDR_MODE);
        break;
    case MEMORY_STATUS_ERR_INVALID_DESTINATION_ADDRESSING_MODE:
        fprintf(stderr, MSG_MEM_ERR_INVALID_DEST_ADDR_MODE);
        break;
    case MEMORY_STATUS_ERR_INVALID_COMMAND:
        fprintf(stderr, MSG_MEM_ERR_INVALID_COMMAND);
        break;
    default:
        fprintf(stderr, MSG_MEM_ERR_UNDEFINED);
        break;
    }
}
