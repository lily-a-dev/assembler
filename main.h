#ifndef ASEMBLER_MAIN_H
#define ASEMBLER_MAIN_H

#include "config.h"

void process_file(char *file_name);
void get_input(FILE *fp, char *file_name);
void read_file(FILE *fp, Data_structs *data_structs, char *param, BOOL is_first_pass);
void init_encoding_pass(FILE *fp, Data_structs *data_structs);
void update_symbols (Data_structs *dataStructs, int icf);
void update_data_img (Data_structs *dataStructs, int icf);

#endif /* ASEMBLER_MAIN_H */
