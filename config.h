#ifndef ASEMBLER_CONFIG_H
#define ASEMBLER_CONFIG_H

#include <stdio.h>

#define MAXLINE 82 /* 82 to allow for \n and \0 */
#define MAXPARAM 32
#define is_space_not_n(c) (((isspace(c)) && ((c)!='\n') && ((c)!='\0'))? (1): (0))
#define CODE_LEN 80 /* used to record the lines from input files for debugging. */
#define OPCODE 6
#define RS_T_D 5
#define FUNCT 5
#define UNUSED 6
#define IMMED 16
#define REG 1
#define ADDRESS 25
#define INPUT_FILE_EXT ".as"
#define COMMENT_INDICATOR ';'
#define LABEL_TERMINATOR ':'
#define ENTRY_STR ".entry"
#define EXTERN_STR ".extern"
#define DH_STR ".dh"
#define DW_STR ".dw"
#define DB_STR  ".db"
#define ASCIZ_STR ".asciz"
#define MAX_REG 31
#define MIN_REG 0



typedef enum {FALSE = 0, TRUE = 1} BOOL;
/* explicitly setting 0,1 values due to extensive use of enums in the program  -
 * making sure we don't need to compare bool values */
typedef enum { I_CMD, R_CMD, J_CMD, NO_CMD } CMD_TYPE;
typedef enum { CMD, LABEL, DH, DW, DB, ASCIZ, ENT, EXT, NONE } PARAM_TYPE;
typedef enum {
    NOT_DEF,
    ADD, SUB, AND, OR, NOR, MOVE, MVHI, MVLO,
    ADDI, SUBI, ANDI, ORI, NORI, BNE, BEQ, BLT,
    BGT, LB, SB, LW, SW, LH, SH, JMP, LA, CALL, STOP
} CMD_NUM;
typedef enum {NOT_CMD, REG_FLAG, ARTH_LOAD_FLAG, COND_FLAG} CMD_FLAG;

typedef enum {
    DATA, CODE, NO_ATTR
} SYMBOL_ATTR;

typedef enum {
    ENT_VIS, EXT_VIS, NO_VIS
} SYMBOL_VIS;

typedef struct Instruction {
    CMD_TYPE instructType;
    CMD_NUM cmd_num;
    char *cmd_name;
    unsigned int opcode: 6;
    unsigned int func: 3;
} Instruction;

typedef struct Hex {
    int bit_num;
    union {
        int whole: 32;
        struct Bits {
            int x1: 8;
            int x2: 8;
            int x3: 8;
            int x4: 8;
        } Hex_bits;
    } num;
} Hex;

typedef struct Symbol_node {
    char *name;
    int value;
    SYMBOL_ATTR attr;
    SYMBOL_VIS vis;
    struct Symbol_node *next;
} Symbol_node;

typedef struct Ext_ent_node {
    char *name;
    int value;
    struct Ext_ent_node *next;
} Ext_ent_node;

typedef struct Data_node {
    int symbol_value;
    char *original_ins;
    PARAM_TYPE param_type;
    int data_count;
    struct Data_node *next;
    void *data;
} Data_node;

typedef struct Line {
    char *data_head;
    char *data;
    int line_num;
    int len;
    int IC;
    int DC;
    BOOL is_error;
} Line;

typedef struct Data_structs {
    char *input_file_name;
    char *hex_file_name;
    Symbol_node *symbol_head;
    Symbol_node *cur_symbol;
    Ext_ent_node *ext_head;
    Ext_ent_node *ext_cur;
    Ext_ent_node *ent_head;
    Ext_ent_node *ent_cur;
    Data_node *data_head;
    Data_node *cur_data;
    Hex *hex;
    Line *line;
    FILE *hex_output_file;
} Data_structs;

typedef struct Cmd_parser_var {
    char *code_bin_head; /* for debug */
    char *code_bin_str; /* for debug */
    long *numbers;
    char *label;
    Instruction cmd_ins;
} Cmd_parser_var;

#endif /*ASEMBLER_CONFIG_H*/


