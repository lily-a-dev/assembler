#include "error_handler.h"
#include "service.h"
#include <stdio.h>

const char error_format[] = "Error. File %s. Line %d: %s.\n";
const char error_op_num[] = "Error. File %s. Line %d: Expecting %d operators.\n";

void print_error(ERROR err, const char *file_name, Line *l) {
    int line_num = l->line_num;
    switch (err) {
        case INPUT_EXCEEDS_MAX_LEN:
            printf(error_format, file_name, line_num, "Line exceeds maximum length of 80 characters");
            break;
        case UNDEFINED_CMD:
            printf(error_format, file_name, line_num, "Undefined command or instruction");
            break;
        case VALUE_OUT_OF_RANGE:
            printf(error_format, file_name, line_num, "Value is out of range for this type of data");
            break;
        case REG_OUT_OF_BOUNDS:
            printf(error_format, file_name, line_num, "A register's number must be between 0-31");
            break;

        case NOT_AN_INT:
            printf(error_format, file_name, line_num, "Expecting an integer");
            break;
        case NOT_A_REG:
            printf(error_format, file_name, line_num, "Expecting a register");
            break;

        case REG_MUST_START_WITH:
            printf(error_format, file_name, line_num, "A register must be preceded be a '$' sign");
            break;
        case MISSING_PARAM:
            printf(error_format, file_name, line_num, "Missing a parameter");
            break;
        case EXTRANEOUS_TEXT:
            printf(error_format, file_name, line_num, "Extraneous text");
            break;
        case MULTIPLE_CONSECUTIVE_COMMAS:
            printf(error_format, file_name, line_num, "Multiple consecutive commas");
            break;
        case MISSING_COMMA:
            printf(error_format, file_name, line_num, "Missing a comma");
            break;
        case ILLEGAL_COMMA:
            printf(error_format, file_name, line_num, "Illegal comma");
            break;


        case STR_MUST_BE_ASCII:
            printf(error_format, file_name, line_num, "String may contain only ASCII characters");
            break;
        case STR_MUST_TERMINATE_WITH_DQ:
            printf(error_format, file_name, line_num, "A string must terminate with double quotes");
            break;
        case STR_MUST_START_WITH_DQ:
            printf(error_format, file_name, line_num, "A string must begin with double quotes");
            break;


        case ILLEGAL_SPACE_BEFORE_COLON:
            printf(error_format, file_name, line_num, "Illegal space before label's colon");
            break;
        case ILLEGAL_SPACE_BETWEEN_REG_AND_NUM:
            printf(error_format, file_name, line_num, "An illegal space between '$' sign and the register's number");
            break;
        case ILLEGAL_SPACE_BETWEEN_DOT:
            printf(error_format, file_name, line_num, "Illegal space between dot and instruction's name");
            break;


        case NOT_A_LABEL:
            printf(error_format, file_name, line_num, "Expecting a label");
            break;
        case LABEL_MUST_START_WITH_ALPHA:
            printf(error_format, file_name, line_num, "Label must start with an alphabet character");
            break;
        case LABEL_ALLOWED_ONLY_ALPHANUM:
            printf(error_format, file_name, line_num, "Label may include only alphanumeric characters");
            break;
        case LABEL_CANNOT_BE_SAVED_WORD:
            printf(error_format, file_name, line_num, "Label may not be a saved word");
            break;
        case LABEL_ALREADY_DEFINED:
            printf(error_format, file_name, line_num, "Label may not be defined more than once");
            break;
        case LABEL_TOO_LONG:
            printf(error_format, file_name, line_num, "Label exceeds maximum length (32 characters, including terminating character)");
            break;
        case LABEL_NOT_PREV_DEFINED:
            printf(error_format, file_name, line_num, "Label hadn't been defined");
            break;
        case LABEL_ALREADY_EXT:
            printf(error_format, file_name, line_num, "Label already defined as external");
            break;
        case LABEL_CANNOT_BE_EXT:
            printf(error_format, file_name, line_num, "Label cannot be defined as external for this type of command");
            break;

        case NO_ERROR:
        case END_OF_LINE:
        default:
            return;

    }
    l->is_error = TRUE;
}

void print_error_no_expected_op(Data_structs *data_structs, int no) {
    data_structs->line->is_error = TRUE;
        printf(error_op_num, data_structs->input_file_name, data_structs->line->line_num, no);
}
void error_line_exceed_length(const char *input_file_name, Line *l, FILE *fp){
    print_error(INPUT_EXCEEDS_MAX_LEN, input_file_name, l);
    while ((l->len = getline(l->data_head, MAXLINE, fp)) > 0)
        if (l->data[l->len-1] == '\n') break;
}
