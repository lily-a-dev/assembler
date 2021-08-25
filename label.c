/*
 * This class encapsulates all functions related to the validation and retrieval of labels (symbols).
 *
 */

#include <string.h>
#include <ctype.h>
#include "label.h"
#include "errors.h"
#include "main.h"
#include "service.h"
#include "error_handler.h"

extern const char *saved_words[];
extern const char LABEL_TERMINATOR;

int get_label(char **line, char *param, BOOL is_colon) {
    int len = 0;
    skip_non_n_whitespace(line);
    if (is_colon && !strchr(*line, LABEL_TERMINATOR)) return 0;
    while (**line != LABEL_TERMINATOR && !isspace(**line)) {
        if (len > MAXPARAM - 1) { return len; }
        param[len++] = *((*line)++);
    }
    if (len < 1) return len;
    param[len] = '\0';
    return len;
}

BOOL is_label_valid(DATA_STRUCTS *data_structs, char *param, BOOL is_colon, BOOL *is_error) {
    int len = get_label(&data_structs->l->data, param, is_colon);
    if (len > (MAXPARAM - 1)) {
        if (is_error) *is_error = TRUE;
        print_error(LABEL_TOO_LONG, data_structs->input_file_name, data_structs->l);
        return FALSE;
    }
    if (len == 0) return FALSE;

    if (is_label_saved_word(data_structs, param) ||
        !is_label_start_alpha(data_structs, param) ||
        !is_label_only_alphanum(data_structs, param) ||
        (is_colon && is_space_before_colon(data_structs))) {
        if (is_error) *is_error = TRUE;
        return FALSE;
    }
    return TRUE;
}

BOOL is_label_saved_word(DATA_STRUCTS *data_structs, char *label){
    int i;
    for (i = 0; saved_words[i]; i++) {
        if ((strcmp(label, saved_words[i])) == 0) {
            print_error(LABEL_CANNOT_BE_SAVED_WORD, data_structs->input_file_name, data_structs->l);
            return TRUE;
        }
    }
    if (get_cmd_index_number(label) != -1){
        print_error(LABEL_CANNOT_BE_SAVED_WORD, data_structs->input_file_name, data_structs->l);
        return TRUE;
    }
    return FALSE;
}

struct symbol_node *get_symbol_node_by_label(char *label, struct symbol_node *head){
    struct symbol_node *cur = head;
    int found;
    do {
        if ((found = strcmp(label, cur->name)) == 0) break;
        cur = cur->next;
    }
    while (cur != NULL);
    if (found == 0) return cur;
    else return NULL;
}

BOOL is_space_before_colon(DATA_STRUCTS *data_structs) {
    if (*data_structs->l->data != ':') {
        print_error(ILLEGAL_SPACE_BEFORE_COLON, data_structs->input_file_name, data_structs->l);
        return TRUE;
    } else { data_structs->l->data++; }
    return FALSE;
}

BOOL is_label_start_alpha(DATA_STRUCTS *data_structs, char *param){
    if (!isalpha(param[0])) {
        print_error(LABEL_MUST_START_WITH_ALPHA, data_structs->input_file_name, data_structs->l);
        return FALSE;
    }
    return TRUE;
}

BOOL is_label_only_alphanum(DATA_STRUCTS *data_structs, char *param){
    int i = 0, len = (int) strlen(param);
    while (i<len) {
        if (!isalnum(param[i++])) {
            print_error(LABEL_ALLOWED_ONLY_ALPHANUM, data_structs->input_file_name, data_structs->l);
            return FALSE;
        }
    }
    return TRUE;
}