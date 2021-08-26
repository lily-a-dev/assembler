#ifndef ASEMBLER_ENCODER_H
#define ASEMBLER_ENCODER_H

#include "config.h"

void second_pass(Data_structs *data_structs, char *param);
void encoder_manager(Data_structs *data_structs, int cmd_in);

void encode_r_cmd (Data_structs *data_structs, Cmd_parser_var *parser_var);
void encode_i_cmd(Data_structs *data_structs, Cmd_parser_var *parser_var);
void encode_j_cmd(Data_structs *data_structs, Cmd_parser_var *parser_var);
BOOL cond_cmd_init (Data_structs *data_structs, Cmd_parser_var *parser_var);

void swap(long *x, long *y);

#endif /* ASEMBLER_ENCODER_H */
