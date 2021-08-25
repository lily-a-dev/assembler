#ifndef ASEMBLER_MEMORY_MANAGER_H
#define ASEMBLER_MEMORY_MANAGER_H

#include "config.h"

DATA_STRUCTS *create_data_structures(char *file_name);
void free_data_struct(DATA_STRUCTS *data_structs);

CMD_PARSER_VAR *allocate_for_parse_fn();
void *create_node(char **param, size_t size, int char_amount);
symbol_node *create_symbol_node();
ext_ent_node *create_ext_ent_node();
data_node *create_data_node();
void init_hex(DATA_STRUCTS *data_structs);
void init_line(DATA_STRUCTS *data_structs);

void free_cmd_parser_var(CMD_PARSER_VAR *parser_var);
void free_symbol_linked_list(symbol_node *head);
void free_ext_ent_linked_list(ext_ent_node *head);
void free_data_linked_list(struct data_node *head);
char * remove_extension(char *file_name);

#endif /* ASEMBLER_MEMORY_MANAGER_H */
