#ifndef ASEMBLER_LABEL_H
#define ASEMBLER_LABEL_H

#include "config.h"

int get_label(char **line, char *param, BOOL is_colon);
BOOL is_label_valid(Data_structs *data_structs, char *param, BOOL is_colon, BOOL *is_error);
BOOL is_label_saved_word(Data_structs *data_structs, char *label);
Symbol_node *get_symbol_node_by_label(char *label, Symbol_node *head);
BOOL is_space_before_colon(Data_structs *data_structs);
BOOL is_label_start_alpha(Data_structs *data_structs, char *param);
BOOL is_label_only_alphanum(Data_structs *data_structs, char *param);

#endif /* ASEMBLER_LABEL_H */
