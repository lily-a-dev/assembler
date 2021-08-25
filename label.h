#ifndef ASEMBLER_LABEL_H
#define ASEMBLER_LABEL_H

#include "config.h"

int get_label(char **line, char *param, BOOL is_colon);
BOOL is_label_valid(DATA_STRUCTS *data_structs, char *param, BOOL is_colon, BOOL *is_error);
BOOL is_label_saved_word(DATA_STRUCTS *data_structs, char *label);
struct symbol_node *get_symbol_node_by_label(char *label, struct symbol_node *head);
BOOL is_space_before_colon(DATA_STRUCTS *data_structs);
BOOL is_label_start_alpha(DATA_STRUCTS *data_structs, char *param);
BOOL is_label_only_alphanum(DATA_STRUCTS *data_structs, char *param);

#endif /* ASEMBLER_LABEL_H */
