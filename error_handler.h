#ifndef ASEMBLER_ERROR_HANDLER_H
#define ASEMBLER_ERROR_HANDLER_H
#include "errors.h"
#include "config.h"

void print_error(ERROR err, const char *file_name, Line *l);
void print_error_no_expected_op(Data_structs *data_structs, int no);
void error_line_exceed_length(const char *file_name, Line *l, FILE *fp);


#endif /* ASEMBLER_ERROR_HANDLER_H */
