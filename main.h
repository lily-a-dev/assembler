#ifndef ASEMBLER_MAIN_H
#define ASEMBLER_MAIN_H

#include "config.h"

int main(int argc, char *argv[]);
void process_file(char *file_name);
void get_input(FILE *fp, char *file_name);
void read_file(FILE *fp, DATA_STRUCTS *data_structs, char *param, BOOL is_first_pass);
void init_encoding_pass(FILE *fp, DATA_STRUCTS *data_structs);
void update_symbols (DATA_STRUCTS *dataStructs, int icf);
void update_data_img (DATA_STRUCTS *dataStructs, int icf);

#endif /* ASEMBLER_MAIN_H */
