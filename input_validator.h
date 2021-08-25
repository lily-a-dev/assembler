#ifndef ASEMBLER_INPUT_VALIDATOR_H
#define ASEMBLER_INPUT_VALIDATOR_H

#include "config.h"
#include "errors.h"

void first_pass (DATA_STRUCTS *data_structs, char *param);
void validate_cmd(DATA_STRUCTS *data_structs, int cmd_in);
void validate_r_cmd (DATA_STRUCTS *data_structs, INSTRUCTION ins);
BOOL validate_r_cmd_param_num(DATA_STRUCTS *data_structs, unsigned int opcode, int count);
void validate_j_cmd (DATA_STRUCTS *data_structs, INSTRUCTION ins);
void validate_init_cond_cmd (DATA_STRUCTS *data_structs, int count);
int insert_data_into_linked_list(DATA_STRUCTS *data_structs, PARAM_TYPE param_type);
void validate_i_cmd (DATA_STRUCTS *data_structs, INSTRUCTION ins);
void process_ext(DATA_STRUCTS *data_structs, BOOL *is_symbol, char *param);
void validate_ent_ext(DATA_STRUCTS *data_structs, BOOL *is_symbol, PARAM_TYPE param_type);
int get_str(DATA_STRUCTS *data_structs, long **data);
void update_data_node(DATA_STRUCTS *data_structs, const long *data, int count, PARAM_TYPE param_type);
BOOL is_db_instruct(PARAM_TYPE type);
BOOL is_extraneous(DATA_STRUCTS *data_structs);
BOOL is_ent_ext(PARAM_TYPE type);

#endif /* ASEMBLER_INPUT_VALIDATOR_H */
