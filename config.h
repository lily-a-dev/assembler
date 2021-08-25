#ifndef ASEMBLER_CONFIG_H
#define ASEMBLER_CONFIG_H

#include <stdio.h>
#define MAXLINE 82 /* 82 to allow for \n and \0 */
#define MAXPARAM 32
#define is_space_not_n(c) (((isspace(c)) && ((c)!='\n') && ((c)!='\0'))? (1): (0))
#define CODE_LEN 80 /* used to record the lines from input files for debugging. */

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

typedef struct {
    CMD_TYPE instructType;
    CMD_NUM cmd_num;
    char *cmd_name;
    unsigned int opcode: 6;
    unsigned int func: 3;
} INSTRUCTION;

struct HEX {
    int bit_num;
    union {
        unsigned long whole: 32;
        struct {
            unsigned long x1: 8;
            unsigned long x2: 8;
            unsigned long x3: 8;
            unsigned long x4: 8;
        } bits;
    };
};

typedef struct symbol_node {
    char *name;
    int value;
    SYMBOL_ATTR attr;
    SYMBOL_VIS vis;
    struct symbol_node *next;
} symbol_node; /*todo - check for ref in technion course */

typedef struct ext_ent_node {
    char *name;
    int value;
    struct ext_ent_node *next;
} ext_ent_node;

typedef struct data_node {
    int symbol_value;
    char *original_ins;
    PARAM_TYPE param_type;
    int data_count;
    struct data_node *next;
    void *data;
} data_node; /*todo - check for ref in technion course */

typedef struct {
    char *data_head;
    char *data;
    int line_num;
    int len;
    int IC;
    int DC;
    BOOL is_error;
} LINE;

typedef struct {
    char *input_file_name;
    char *hex_file_name;
    struct symbol_node *symbol_head;
    struct symbol_node *cur_symbol;
    struct ext_ent_node *ext_head;
    struct ext_ent_node *ext_cur;
    struct ext_ent_node *ent_head;
    struct ext_ent_node *ent_cur;
    struct data_node *data_head;
    struct data_node *cur_data;
    struct HEX *hex;
    LINE *l;
    FILE *hex_output_file;
} DATA_STRUCTS;

typedef struct {
    char *code_bin_head; /* for debug */
    char *code_bin_str; /* for debug */
    long *numbers;
    char *label;
    INSTRUCTION cmd_ins;
} CMD_PARSER_VAR;

#endif /*ASEMBLER_CONFIG_H*/


