/*
 * This class preforms a few more additional validations that couldn't
 * be preformed without data from the first pass.
 * Additionally, it encodes the assembler code to hex-code.
 * Note, there's an additional function that prints the assembler code
 * and its corresponding binary code.
 * The function is useful for debugging, and can be toggled ON/OFF in config.c.
 *
 */


#include <stdlib.h>
#include <string.h>
#include "encoder.h"
#include "service.h"
#include "memory_manager.h"
#include "output_manager.h"
#include "error_handler.h"
#include "label.h"

extern const char binary_print_format[];
extern const char *saved_words[];
extern Instruction instruct_array[];
extern BOOL IS_DEBUG;

void second_pass(Data_structs *data_structs, char *param){
    Line *l = data_structs->line;
    Symbol_node *symbol;
    PARAM_TYPE param_type;

    while (*l->data && *l->data != '\n') {
        param_type = get_param(data_structs, param, FALSE);
        if (param_type == LABEL) { continue; }
        else if (param_type == CMD) {
            encoder_manager(data_structs, get_cmd_index_number(param));
            if (data_structs->line->is_error) return;
        } else if (get_db_ent_type(param) != ENT){
            return;
        } else if (param_type == ENT){
            get_label(&data_structs->line->data, param, FALSE);
            symbol = get_symbol_node_by_label(param, data_structs->symbol_head);
            if (symbol == NULL) {
                print_error(LABEL_NOT_PREV_DEFINED, data_structs->input_file_name, data_structs->line);
            } else {
                if (symbol->vis == EXT_VIS) {
                    print_error(LABEL_ALREADY_EXT, data_structs->input_file_name,
                                data_structs->line);
                } else {
                    symbol->vis = ENT_VIS;
                    strcpy(data_structs->ent_cur->name,symbol->name);
                    data_structs->ent_cur->value = symbol->value;
                    data_structs->ent_cur->next = create_ext_ent_node(); /* create_ext_ent_node is allocation safe */
                    data_structs->ent_cur = data_structs->ent_cur->next;
                }
            }
            return;
        }
    }
}

void encoder_manager(Data_structs *data_structs, int cmd_in){
    struct Bits *hex_bits = &data_structs->hex->num.Hex_bits;
    Cmd_parser_var *parser_var = allocate_for_parse_fn(); /* allocation safe */
    parser_var->cmd_ins = instruct_array[cmd_in];
    encode_to_bin(&parser_var->code_bin_str, OPCODE, instruct_array[cmd_in].opcode);
    encode_to_hex(data_structs->hex, OPCODE, instruct_array[cmd_in].opcode);
    switch (instruct_array[cmd_in].instructType) {
        case R_CMD:
            encode_r_cmd(data_structs, parser_var);
            break;
        case I_CMD:
            encode_i_cmd(data_structs, parser_var);
            break;
        case J_CMD:
            encode_j_cmd(data_structs, parser_var);
            break;
        case NO_CMD:
        default:
            printf("Unknown error at parse_cmd fn. Line %d.\n", data_structs->line->line_num); /* for debugging */

    }
    if (!data_structs->line->is_error) {
        print_binary_to_stdout(data_structs, parser_var);
        fprintf(data_structs->hex_output_file, "\n%04d %02x %02x %02x %02x", data_structs->line->IC, data_structs->hex->num.Hex_bits.x1&0xff,
                data_structs->hex->num.Hex_bits.x2&0xff, data_structs->hex->num.Hex_bits.x3&0xff, data_structs->hex->num.Hex_bits.x4&0xff);
        data_structs->hex->num.whole = 0;
        data_structs->hex->bit_num = 31;
        data_structs->line->IC += 4;
    }
    skip_non_n_whitespace(&data_structs->line->data);
    free_cmd_parser_var(parser_var);
}

void encode_r_cmd (Data_structs *data_structs, Cmd_parser_var *parser_var){
    int j=0;
    long *numbers = parser_var->numbers;
    Instruction ins = parser_var->cmd_ins;
    get_all_numbers(data_structs, &parser_var->numbers, REG_FLAG, NONE);
    if (ins.opcode == (unsigned) 1) {
        numbers[2] = numbers[1];
        numbers[1] = 0;
    }
    for (; j<3; j++){
        encode_to_bin(&parser_var->code_bin_str, RS_T_D, numbers[j]);
        encode_to_hex(data_structs->hex, RS_T_D, numbers[j]);
    }
    encode_to_bin(&parser_var->code_bin_str, FUNCT, ins.func);
    encode_to_bin(&parser_var->code_bin_str, UNUSED, 0);
    encode_to_hex(data_structs->hex,FUNCT, ins.func);
    encode_to_hex(data_structs->hex, UNUSED, 0);
}

void encode_i_cmd (Data_structs *data_structs, Cmd_parser_var *parser_var){
    int count, i = 0;
    CMD_FLAG flag = is_condi(parser_var->cmd_ins.cmd_num) ? COND_FLAG : ARTH_LOAD_FLAG;
    count = get_all_numbers(data_structs, &parser_var->numbers, flag, NONE);

    if (flag == ARTH_LOAD_FLAG) {
        swap(&parser_var->numbers[1], &parser_var->numbers[2]);
        count = 2;
    } else if (!cond_cmd_init(data_structs, parser_var)){
        return;
    }
    for (; i < count; i++){
        encode_to_bin(&parser_var->code_bin_str, RS_T_D, parser_var->numbers[i]);
        encode_to_hex(data_structs->hex,RS_T_D, parser_var->numbers[i]);
    }
    encode_to_bin(&parser_var->code_bin_str, IMMED, parser_var->numbers[2]);
    encode_to_hex(data_structs->hex,IMMED, parser_var->numbers[2]);
}

void encode_j_cmd(Data_structs *data_structs, Cmd_parser_var *parser_var) {
    Symbol_node *symbol;
    long reg;

    Line *l = data_structs->line;
    skip_non_n_whitespace(&l->data);
    if (parser_var->cmd_ins.cmd_num == JMP && *l->data == '$') {
        l->data++;
        get_register(data_structs, &reg);
        encode_to_bin(&parser_var->code_bin_str, REG, 1);
        encode_to_bin(&parser_var->code_bin_str, ADDRESS, reg);
        encode_to_hex(data_structs->hex,REG, 1);
        encode_to_hex(data_structs->hex,ADDRESS, reg);
        return;
    }
    encode_to_bin(&parser_var->code_bin_str, REG, 0);
    encode_to_hex(data_structs->hex, REG, 0);
    if (parser_var->cmd_ins.cmd_num == STOP) {
        encode_to_bin(&parser_var->code_bin_str, ADDRESS, 0);
        encode_to_hex(data_structs->hex, ADDRESS, 0);
        return;
    }
    get_label(&l->data, parser_var->label, FALSE);
    symbol = get_symbol_node_by_label(parser_var->label, data_structs->symbol_head);
    if (symbol == NULL) {
        print_error(LABEL_NOT_PREV_DEFINED, data_structs->input_file_name, data_structs->line);
        return;
    } else {
        if (symbol->vis == EXT_VIS){
            strcpy(data_structs->ext_cur->name,symbol->name);
            data_structs->ext_cur->value = l->IC;
            data_structs->ext_cur->next = create_ext_ent_node();
            data_structs->ext_cur = data_structs->ext_cur->next; /* create_ext_ent_node is allocation sfe */
        }
        encode_to_bin(&parser_var->code_bin_str, ADDRESS, symbol->value);
        encode_to_hex(data_structs->hex, ADDRESS,  symbol->value);
    }
}

BOOL cond_cmd_init (Data_structs *data_structs, Cmd_parser_var *parser_var){
    Symbol_node *symbol;
    get_label(&data_structs->line->data, parser_var->label, FALSE);
    symbol = get_symbol_node_by_label(parser_var->label, data_structs->symbol_head);
    if (symbol == NULL) {
        print_error(LABEL_NOT_PREV_DEFINED, data_structs->input_file_name, data_structs->line);
        return FALSE;
    }
    if (symbol->vis == EXT_VIS){
        print_error(LABEL_CANNOT_BE_EXT, data_structs->input_file_name, data_structs->line);
        return FALSE;
    }
    if (symbol->attr == CODE)
        parser_var->numbers[2] = symbol->value - data_structs->line->IC;
    else { parser_var->numbers[2] = symbol->value - data_structs->line->DC; }

    return TRUE;

}

void swap(long *x, long *y) {
    long temp = *x;
    *x = *y;
    *y = temp;
}