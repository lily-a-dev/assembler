#ifndef ASEMBLER_INPUT_VALIDATOR_H
#define ASEMBLER_INPUT_VALIDATOR_H

#include "config.h"
#include "errors.h"

void first_pass (Data_structs *data_structs, char *param);
void validate_cmd(Data_structs *data_structs, int cmd_in);
void validate_r_cmd (Data_structs *data_structs, Instruction ins);
BOOL validate_r_cmd_param_num(Data_structs *data_structs, unsigned int opcode, int count);
void validate_j_cmd (Data_structs *data_structs, Instruction ins);
void validate_init_cond_cmd (Data_structs *data_structs, int count);
int insert_data_into_linked_list(Data_structs *data_structs, PARAM_TYPE param_type);
void validate_i_cmd (Data_structs *data_structs, Instruction ins);
void process_ext(Data_structs *data_structs, BOOL *is_symbol, char *param);
void validate_ent_ext(Data_structs *data_structs, BOOL *is_symbol, PARAM_TYPE param_type);
int get_str(Data_structs *data_structs, long **data);
void update_data_node(Data_structs *data_structs, const long *data, int count, PARAM_TYPE param_type);
BOOL is_db_instruct(PARAM_TYPE type);
BOOL is_extraneous(Data_structs *data_structs);
BOOL is_ent_ext(PARAM_TYPE type);

#endif /* ASEMBLER_INPUT_VALIDATOR_H */
