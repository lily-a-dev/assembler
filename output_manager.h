#ifndef ASEMBLER_OUTPUT_MANAGER_H
#define ASEMBLER_OUTPUT_MANAGER_H

#include <stdio.h>
#include "config.h"

void print_ob_file_header(DATA_STRUCTS *data_structs, int icf, int dcf);
void print_data_to_file(DATA_STRUCTS *data_structs);
void print_ext_ent_manager(DATA_STRUCTS *data_structs);
void print_ext_ent_to_file(ext_ent_node *head, char *file_name);
void print_hex_to_file(DATA_STRUCTS *data_structs, unsigned long num, int bits_num, int *cells, int *line_num);
void encode_to_hex(struct HEX *hex, int no_bits, unsigned long to_encode);
void print_binary_to_stdout (DATA_STRUCTS *data_structs, CMD_PARSER_VAR *parser_var);
void encode_to_bin(char **encode_dest, int no_bits, unsigned int to_encode);

#endif /* ASEMBLER_OUTPUT_MANAGER_H */
