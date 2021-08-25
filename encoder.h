#ifndef ASEMBLER_ENCODER_H
#define ASEMBLER_ENCODER_H

#include "config.h"

void second_pass(DATA_STRUCTS *data_structs, char *param);
void encoder_manager(DATA_STRUCTS *data_structs, int cmd_in);

void encode_r_cmd (DATA_STRUCTS *data_structs, CMD_PARSER_VAR *parser_var);
void encode_i_cmd(DATA_STRUCTS *data_structs, CMD_PARSER_VAR *parser_var);
void encode_j_cmd(DATA_STRUCTS *data_structs, CMD_PARSER_VAR *parser_var);
BOOL cond_cmd_init (DATA_STRUCTS *data_structs, CMD_PARSER_VAR *parser_var);

void swap(long *x, long *y);

#endif /* ASEMBLER_ENCODER_H */
