#ifndef ASEMBLER_SERVICE_H
#define ASEMBLER_SERVICE_H

#include <stdio.h>
#include "config.h"
#include "errors.h"

PARAM_TYPE get_param(DATA_STRUCTS *data_structs, char *param, BOOL is_first_pass);
int get_all_numbers(DATA_STRUCTS *data_structs, long **numbers, CMD_FLAG flag, PARAM_TYPE param_type);
ERROR get_single_number(long **numbers, int *count, char **l_start, BOOL is_reg, PARAM_TYPE param_type);
PARAM_TYPE is_db_ent(DATA_STRUCTS *data_structs, char *param, BOOL *is_error);
PARAM_TYPE get_db_ent_type(char *param);
BOOL get_cmd_param(LINE *l, char *param);
int get_cmd_index_number(char *cmd_str);
ERROR validate_reg_init(char **l_start);
BOOL is_condi(CMD_NUM cmdNum);
ERROR init_validation(char **l_start, char **l_end, CMD_FLAG flag);
char *add_extension_to_file(char *file_name, const char *ext);
int get_data_size(PARAM_TYPE paramType);
ERROR validate_range(long num, BOOL is_reg, PARAM_TYPE param_type);
void skip_non_n_whitespace(char **str);
int getline(char *line, int max, FILE *fp);
BOOL get_register(DATA_STRUCTS *data_structs, long *reg);


#endif /* ASEMBLER_SERVICE_H */
