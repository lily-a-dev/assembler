#ifndef ASEMBLER_OUTPUT_MANAGER_H
#define ASEMBLER_OUTPUT_MANAGER_H

#include <stdio.h>
#include "config.h"

void print_ob_file_header(Data_structs *data_structs, int icf, int dcf);
void print_data_to_file(Data_structs *data_structs);
void print_ext_ent_manager(Data_structs *data_structs);
void print_ext_ent_to_file(Ext_ent_node *head, char *file_name);
void print_hex_to_file(Data_structs *data_structs, long num, int bits_num, int *cells, int *line_num);
void encode_to_hex(Hex *hex, int no_bits, long to_encode);
void print_binary_to_stdout (Data_structs *data_structs, Cmd_parser_var *parser_var);
void encode_to_bin(char **encode_dest, int no_bits, unsigned int to_encode);

#endif /* ASEMBLER_OUTPUT_MANAGER_H */
