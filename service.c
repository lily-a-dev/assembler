#include <string.h>
#include "service.h"
#include "error_handler.h"
#include "label.h"
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

extern Instruction instruct_array[];

PARAM_TYPE get_param(Data_structs *data_structs, char *param, BOOL is_first_pass) {
    PARAM_TYPE type;
    BOOL is_error = FALSE;
    type = is_db_ent(data_structs, param, &is_error);
    if (type != NONE)
        return type;
    if (is_first_pass && is_label_valid(data_structs, param, TRUE, &is_error)) {
        if (get_symbol_node_by_label(param, data_structs->symbol_head) != NULL) {
            print_error(LABEL_ALREADY_DEFINED, data_structs->input_file_name, data_structs->line);
            return NONE;
        }
        return LABEL;
    }
    if (!is_first_pass && (get_label(&data_structs->line->data, param, TRUE) > 0)) {
        data_structs->line->data++;
        return LABEL;
    }
    if (get_cmd_param(data_structs->line, param)) {
        return CMD;
    }
    if (!is_error)print_error(UNDEFINED_CMD, data_structs->input_file_name, data_structs->line);

    return NONE;
}

int get_all_numbers(Data_structs *data_structs, long **numbers, CMD_FLAG flag, PARAM_TYPE param_type) {
    ERROR status = NO_ERROR;
    int count = 0;
    char *l_start = data_structs->line->data, *l_end = &data_structs->line->data_head[data_structs->line->len - 1];
    BOOL is_reg;

    skip_non_n_whitespace(&l_start);
    while (isspace(*l_end)) (l_end)--;
    if (!numbers){
        status = init_validation(&l_start, &l_end, flag);
        if (status != NO_ERROR) {
            print_error(status, data_structs->input_file_name, data_structs->line);
            return 0;
        }
    }

    while (count < MAXLINE && l_start <= l_end){
        is_reg = (flag == REG_FLAG || flag == COND_FLAG || (flag == ARTH_LOAD_FLAG && (count == 0 || count == 2)));
        skip_non_n_whitespace(&l_start);
        if (!numbers && is_reg && ((status = validate_reg_init(&l_start)) != NO_ERROR)) { break; }
        if (numbers &&is_reg) l_start++; /*skipping $ sign */
        status = get_single_number(numbers, &count, &l_start, is_reg, param_type);
        if (status != NO_ERROR) { break; }
        if (*l_start == '.') { status = NOT_AN_INT; break; }
        if (flag != NOT_CMD && count > 2) break;
        if (*l_start != ','){ status = MISSING_COMMA; break; }
        l_start++;
        if (flag == COND_FLAG && count == 2) break;
    }

    if (status != END_OF_LINE && status != NO_ERROR) {
        print_error(status, data_structs->input_file_name, data_structs->line);
        return 0;
    }
    data_structs->line->data = l_start;
    return count;
}

ERROR validate_number_syntax(char **l_start, BOOL is_activate_flag){
    if (**l_start == '\n') { return END_OF_LINE; }
    if (**l_start == ',') { return MULTIPLE_CONSECUTIVE_COMMAS; }
    else {
        if (is_activate_flag) return NOT_A_REG;
        else return NOT_AN_INT;
    }
}

ERROR validate_range(long num, BOOL is_reg, PARAM_TYPE param_type) {
    if (errno == ERANGE) { errno = 0; return VALUE_OUT_OF_RANGE; }
    if (is_reg && (num < MIN_REG || num > MAX_REG))
        return REG_OUT_OF_BOUNDS;
    if (param_type == DH && (num < SHRT_MIN || num > SHRT_MAX))
        return VALUE_OUT_OF_RANGE;
    if (param_type == DB && (num < CHAR_MIN || num > CHAR_MAX))
        return VALUE_OUT_OF_RANGE;
    return NO_ERROR;
}

ERROR get_single_number(long **numbers, int *count, char **l_start, BOOL is_reg, PARAM_TYPE param_type) {
    long num;
    char *endptr = 0;
    ERROR err = NO_ERROR;
    num = strtol(*l_start, &endptr, 10);
    if (!numbers || !is_reg) {
        err = (*l_start == endptr) ? /* failed to retrieve a number */
            validate_number_syntax(l_start, is_reg) :
            validate_range(num, is_reg, param_type);
    }
    if (numbers && *l_start != endptr) {
        (*numbers)[(*count)] = num;
    }
    if (err != NO_ERROR) return err;
    (*count)++;
    *l_start = endptr;
    skip_non_n_whitespace(l_start);
    if (**l_start == '\n') { return END_OF_LINE; }
    return NO_ERROR;
}

PARAM_TYPE is_db_ent(Data_structs *data_structs, char *param, BOOL *is_error) {
    int i = 0;
    Line *l = data_structs->line;
    skip_non_n_whitespace(&l->data);
    if (*l->data != '.') return NONE;
    if (isspace(*(l->data+1))) {
        if (is_error) *is_error = TRUE;
        print_error(ILLEGAL_SPACE_BETWEEN_DOT, data_structs->input_file_name, l);
        return NONE;
    }
    while (i < MAXPARAM && !isspace(*l->data) && *l->data != ',') {
        param[i++] = *(l->data++);
    }
    param[i] = '\0';
    return get_db_ent_type(param);
}

PARAM_TYPE get_db_ent_type(char *param) {
    if ((strcmp(param, DB_STR))==0) return DB;
    if ((strcmp(param, DH_STR))==0) return DH;
    if ((strcmp(param, DW_STR))==0) return DW;
    if ((strcmp(param, ASCIZ_STR))==0) return ASCIZ;
    if ((strcmp(param, ENTRY_STR))==0) return ENT;
    if ((strcmp(param, EXTERN_STR))==0) return EXT;

    return NONE;
}

BOOL get_cmd_param(Line *l, char *param){
    int i = 0;
    while (i < MAXPARAM && !isspace(*l->data) && *l->data != ',' && *l->data != '\0'){
        param[i++] = *(l->data++);
    }
    param[i]='\0';
    return get_cmd_index_number(param) > -1;
}

int get_cmd_index_number(char *cmd_str) {
    int i;
    for (i = 0; instruct_array[i].cmd_name; i++) {
        if ((strcmp(cmd_str, instruct_array[i].cmd_name)) == 0) {
            return i;
        }
    }
    return -1;
}



ERROR validate_reg_init(char **l_start){
    ERROR status = NO_ERROR;
    if (**l_start == ',') { status = MULTIPLE_CONSECUTIVE_COMMAS; }
    else if (**l_start != '$') { status = REG_MUST_START_WITH; }
    else if (isspace(*++(*l_start))) { status = ILLEGAL_SPACE_BETWEEN_REG_AND_NUM; }
    return status;
}

ERROR init_validation(char **l_start, char **l_end, CMD_FLAG flag){
    if (*l_start > *l_end) return MISSING_PARAM;
    if (flag != NOT_CMD && **l_start != '$') return NOT_A_REG;
    else if (flag == NOT_CMD && **l_start != '+' && **l_start != '-' && !isdigit(**l_start)) { return NOT_AN_INT; }
    if (!isdigit(**l_end)) {
        if (flag == COND_FLAG && isalnum(**l_end)) return NO_ERROR;
        if (flag != COND_FLAG &&**l_end == ',') return ILLEGAL_COMMA;
    }
    return NO_ERROR;
}

int get_data_size(PARAM_TYPE paramType){
    /* .db - 1 byte; .dw - 4 bytes. .dh - 2 bytes. */
    switch (paramType) {
        case DB:
            return 1;
        case DH:
            return 2;
        case DW:
            return 4;
        case ASCIZ:
            return 1;
        default:
            return 0;
    }
}

char *add_extension_to_file(char *file_name, const char *ext){
    char *new_file = calloc(MAXLINE, sizeof(char));
    if (new_file) {
        strcpy(new_file, file_name);
        strcat(new_file, ext);
    } else {
        exit(EXIT_FAILURE);
    }
    return new_file;
}

void skip_non_n_whitespace(char **str){
    while (is_space_not_n(**str)) { (*str)++; }
}

int getline(char *line, int max, FILE *fp) {
    /* fgets:
     * stops when either (n-1) characters are read, the newline character is read,
     * or the end-of-file is reached, whichever comes first.
     * it stores the \n and adds \0
    */

    if (fgets(line, max, fp) == NULL)
        return 0;
    else
        return strlen(line);
}

BOOL is_condi(CMD_NUM cmdNum){
    /* not checking  ADDI <= cmdNum <=NORI,
     * because a careless change in the order of CMD_NUM
     * definition might affect the validity of the fn */
    return (cmdNum == BLT ||
            cmdNum == BGT ||
            cmdNum == BEQ ||
            cmdNum == BNE );
}


BOOL get_register(Data_structs *data_structs, long *reg) {
    Line *l = data_structs->line;
    char *endptr;
    *reg = strtol(l->data, &endptr, 10);
    if (l->data == endptr){
        if (isspace(*++l->data)) {
            print_error(ILLEGAL_SPACE_BETWEEN_REG_AND_NUM, data_structs->input_file_name, l);
            return FALSE;
        }
    }
    else if (validate_range(*reg, TRUE ,NONE) != NO_ERROR){
        return FALSE;
    }
    l->data = endptr;
    return TRUE;
}
