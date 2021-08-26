/*
 * This class preforms validation for various syntax and value errors.
 * Additionally, it saves any valid data (i.e. .db, .dw, .dh and .asciz data),
 * and builds a symbol tables that's used for further validation in the second pass.
 *
 * Some of the validation is delegated to dedicated classes, or to a general program-wide class.
 *
 */


#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "input_validator.h"
#include "service.h"
#include "memory_manager.h"
#include "label.h"
#include "error_handler.h"

extern Instruction instruct_array[];

void first_pass (Data_structs *data_structs, char *param) {
    BOOL is_symbol = FALSE;
    Line *l = data_structs->line;
    Symbol_node *cur_symbol = data_structs->cur_symbol;
    PARAM_TYPE param_type;
    int cmd_in;

    while (*l->data && *l->data != '\n') {
        param_type = get_param(data_structs, param, TRUE);
        if (param_type == NONE) break;
        skip_non_n_whitespace(&l->data);
        if (*l->data == ',') {
            print_error(ILLEGAL_COMMA, data_structs->input_file_name, l);
            break;
        }
        if (param_type == LABEL) {
            is_symbol = TRUE;
            strncpy(cur_symbol->name, param, MAXPARAM);
            continue;
        }
        else if (is_db_instruct(param_type)) {
            if (is_symbol) {
                cur_symbol->attr = DATA;
                cur_symbol->value = l->DC;
            }
            l->DC += insert_data_into_linked_list(data_structs, param_type);
            break;
        } else if (is_ent_ext(param_type)) {
            validate_ent_ext(data_structs, &is_symbol, param_type);
            break;
        }
        else { /* line is code */
            cmd_in = get_cmd_index_number(param);
            validate_cmd(data_structs, cmd_in);
            if (is_symbol){
                cur_symbol->attr = CODE;
                cur_symbol->value = l->IC;
            }
            l->IC += 4;
            break;
        }
    }

    if (is_symbol) {
        is_symbol = FALSE;
        cur_symbol->next = create_symbol_node(); /*create_symbol_node is allocation safe */
        data_structs->cur_symbol = cur_symbol->next;
    }

}

void validate_cmd(Data_structs *data_structs, int cmd_in){
    switch (instruct_array[cmd_in].instructType) {
        case R_CMD:
            validate_r_cmd(data_structs, instruct_array[cmd_in]);
            break;
        case I_CMD:
            validate_i_cmd(data_structs, instruct_array[cmd_in]);
            break;
        case J_CMD:
            validate_j_cmd(data_structs, instruct_array[cmd_in]);
            break;
        case NO_CMD:
            break;
    }
}

void validate_r_cmd (Data_structs *data_structs, Instruction ins){
    int count;
    count = get_all_numbers(data_structs, NULL, REG_FLAG, NONE);
    if (validate_r_cmd_param_num(data_structs, ins.opcode, count))
        is_extraneous(data_structs);
}

BOOL validate_r_cmd_param_num(Data_structs *data_structs, unsigned int opcode, int count) {
    if (count == 0) return FALSE;
    if (opcode == 0 && count != 3) {
        print_error_no_expected_op(data_structs, 3);
        return FALSE;
    }
    if (opcode == 1 && count != 2) {
        print_error_no_expected_op(data_structs, 2);
        return FALSE;
    }
    return TRUE;
}

void validate_j_cmd (Data_structs *data_structs, Instruction ins) {
    Line *l = NULL;
    long reg;
    char *label = calloc(MAXPARAM, sizeof(char));
    if (!label) { exit(EXIT_FAILURE); }
    l = data_structs->line;
    skip_non_n_whitespace(&l->data);
    if (ins.cmd_num != STOP && *l->data == '\n') {
        free(label);
        print_error(MISSING_PARAM, data_structs->input_file_name, l);
        return;
    }
    if (ins.cmd_num == JMP && *l->data == '$') {
        l->data++;
        if (!get_register(data_structs, &reg)) {
            free(label);
            return;
        }
    } else if (ins.cmd_num != STOP && !is_label_valid(data_structs, label, FALSE, NULL)) {
        /*problem with label -- handled inside is_label_valid fn. */
        free(label);
        return;
    }

    if (is_extraneous(data_structs)){
        free(label);
        return;
    }
    free(label);
}

void validate_i_cmd (Data_structs *data_structs, Instruction ins){
    int count;
    CMD_FLAG flag = is_condi(ins.cmd_num) ? COND_FLAG : ARTH_LOAD_FLAG;
    count = get_all_numbers(data_structs, NULL, flag, NONE);
    if (count == 0) return;
    if (flag == ARTH_LOAD_FLAG) {
        if (count != 3) {
            print_error_no_expected_op(data_structs, 3);
        }
    } else {
        validate_init_cond_cmd(data_structs, count);
    }
}

void validate_init_cond_cmd (Data_structs *data_structs, int count){
    char *label = calloc(MAXPARAM, sizeof(char));
    if (!label) { exit(EXIT_FAILURE); }
    if (count != 2) {
        print_error_no_expected_op(data_structs, 2);
    }
    is_label_valid(data_structs, label, FALSE, NULL);
    if (*label == '\0') print_error(MISSING_PARAM, data_structs->input_file_name, data_structs->line);
    is_extraneous(data_structs);
    free(label);
}

int insert_data_into_linked_list(Data_structs *data_structs, PARAM_TYPE param_type){
    int count, data_size = get_data_size(param_type);
    long *data = calloc(MAXLINE, sizeof(long));
    if (!data) { exit(EXIT_FAILURE); }

    count = (param_type == ASCIZ) ? get_str(data_structs, &data) : get_all_numbers(
            data_structs, &data, NOT_CMD, param_type);
    if (count != 0 && is_extraneous(data_structs)) return 0;
    update_data_node(data_structs, data, count, param_type);

    free(data);
    return count * data_size;
}

void process_ext(Data_structs *data_structs, BOOL *is_symbol, char *param){
    Symbol_node *symbol = get_symbol_node_by_label(param, data_structs->symbol_head);
    if (symbol != NULL && symbol->vis != EXT_VIS) {
        print_error(LABEL_ALREADY_DEFINED, data_structs->input_file_name, data_structs->line);
    } else {
        *is_symbol = TRUE;
        strncpy(data_structs->cur_symbol->name, param, MAXPARAM);
        data_structs->cur_symbol->value = 0;
        data_structs->cur_symbol->vis = EXT_VIS;
    }
}

void validate_ent_ext(Data_structs *data_structs, BOOL *is_symbol, PARAM_TYPE param_type){
    char *param = calloc(MAXPARAM, sizeof(char));
    if (!param) { exit(EXIT_FAILURE); }

    else if (is_label_valid(data_structs, param, FALSE, NULL)){
        if (param_type == EXT) {
            process_ext(data_structs, is_symbol, param);
        }
    } else {
        print_error(NOT_A_LABEL, data_structs->input_file_name, data_structs->line);
    }
    free(param);
}

int get_str(Data_structs *data_structs, long **data) {
    int i = 0;
    char *last;
    Line *l = data_structs->line;

    skip_non_n_whitespace(&l->data);
    if (*l->data == '\n') {
        print_error(MISSING_PARAM, data_structs->input_file_name, l);
        return 0;
    }
    if (*l->data != '\"') {
        print_error(STR_MUST_START_WITH_DQ, data_structs->input_file_name, l);
        return 0;
    }
    l->data++;
    last = strrchr(l->data, '\"');

    if (last == NULL) { print_error(STR_MUST_TERMINATE_WITH_DQ, data_structs->input_file_name, l); return 0; }

    while (l->data < last){
        if (!isprint(*l->data)) {
            print_error(STR_MUST_BE_ASCII, data_structs->input_file_name, l);
            return 0;
        }
        (*data)[i++] = (unsigned char) *(l->data++);
    }
    (*data)[i++] = '\0';

    l->data = ++last;
    return i;
}

void update_data_node(Data_structs *data_structs, const long *data, int count, PARAM_TYPE param_type){
    int i = 0;

    switch (param_type) {
        /* .db - 1 byte; .dw - 4 bytes. .dh - 2 bytes. */
        case DH:
            data_structs->cur_data->data = calloc(count, sizeof(int));
            if (!data_structs->cur_data->data) { exit(EXIT_FAILURE); }
            for (; i<count; i++) {  ((int*) data_structs->cur_data->data)[i] = (int) data[i]; }
            break;
        case DW:
            data_structs->cur_data->data = calloc(count, sizeof(long));
            if (!data_structs->cur_data->data) { exit(EXIT_FAILURE); }
            for (; i<count; i++) { ((long *) data_structs->cur_data->data)[i] = (long) data[i]; }
            break;
        case DB:
        case ASCIZ:
            data_structs->cur_data->data = calloc(count, sizeof(char));
            if (!data_structs->cur_data->data) { exit(EXIT_FAILURE); }
            for (; i<count; i++) { ((char *) data_structs->cur_data->data)[i] = (char) data[i]; }
            break;
        case CMD: case LABEL: case ENT: case EXT: case NONE:
        default:
            break;
    }

    data_structs->cur_data->param_type = param_type;
    data_structs->cur_data->symbol_value = data_structs->line->DC;
    strncpy(data_structs->cur_data->original_ins, data_structs->line->data_head, data_structs->line->len - 1); /* this line is for debugging */
    data_structs->cur_data->data_count = count;
    data_structs->cur_data->next = create_data_node(); /*create_data_node is allocation safe */


    data_structs->cur_data = data_structs->cur_data->next;
}

BOOL is_db_instruct(PARAM_TYPE type){
    return (BOOL) (type == DB || type == DH ||
                   type == ASCIZ || type == DW);
}

BOOL is_extraneous(Data_structs *data_structs) {
    skip_non_n_whitespace(&data_structs->line->data);
    if (*data_structs->line->data != '\n' && *data_structs->line->data != '\0' ) {
        print_error(EXTRANEOUS_TEXT, data_structs->input_file_name, data_structs->line);
        return TRUE;
    }
    return FALSE;
}

BOOL is_ent_ext(PARAM_TYPE type) {
    return (BOOL) (type == ENT || type == EXT);
}
