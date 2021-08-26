#ifndef ASEMBLER_MEMORY_MANAGER_H
#define ASEMBLER_MEMORY_MANAGER_H

#include "config.h"

Data_structs *create_data_structures(char *file_name);
void free_data_struct(Data_structs *data_structs);

Cmd_parser_var *allocate_for_parse_fn();
void *create_node(char **param, size_t size, int char_amount);
Symbol_node *create_symbol_node();
Ext_ent_node *create_ext_ent_node();
Data_node *create_data_node();
void init_hex(Data_structs *data_structs);
void init_line(Data_structs *data_structs);

void free_cmd_parser_var(Cmd_parser_var *parser_var);
void free_symbol_linked_list(Symbol_node *head);
void free_ext_ent_linked_list(Ext_ent_node *head);
void free_data_linked_list(Data_node *head);
char * remove_extension(char *file_name);

#endif /* ASEMBLER_MEMORY_MANAGER_H */
