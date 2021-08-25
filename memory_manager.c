#include <stdio.h>
#include <stdlib.h>
#include "memory_manager.h"
#include "string.h"
#include "service.h"

extern const char *INPUT_FILE_EXT;
extern const BOOL IS_DEBUG;

DATA_STRUCTS *create_data_structures(char *file_name) {
    DATA_STRUCTS *data_structs = calloc(1, sizeof(DATA_STRUCTS));
    if (data_structs==NULL) { exit(EXIT_FAILURE); }

    file_name = remove_extension(file_name);
    if (file_name == NULL) /* not .as file */ return NULL;
    data_structs->input_file_name = file_name;
    data_structs->hex_file_name = add_extension_to_file(data_structs->input_file_name, ".ob");
    data_structs->hex_output_file = fopen(data_structs->hex_file_name, "w+");
    init_hex(data_structs);
    init_line(data_structs);

    symbol_node *symbol_head = create_symbol_node();
    data_node *data_head = create_data_node();
    ext_ent_node *ext_head = create_ext_ent_node();
    ext_ent_node *ent_head = create_ext_ent_node();
    data_structs->symbol_head = symbol_head;
    data_structs->cur_symbol = symbol_head;
    data_structs->data_head = data_head;
    data_structs->cur_data = data_head;
    data_structs->ext_head = ext_head;
    data_structs->ext_cur = ext_head;
    data_structs->ent_head = ent_head;
    data_structs->ent_cur = ent_head;

    return data_structs;
}

void free_data_struct(DATA_STRUCTS *data_structs){
    if (data_structs->input_file_name) free(data_structs->input_file_name);
    if (data_structs->hex_file_name) free(data_structs->hex_file_name);
    if (data_structs->hex_output_file) fclose(data_structs->hex_output_file);
    if (data_structs->hex) free(data_structs->hex);
    if (data_structs->l) {
        if (data_structs->l->data_head) { free(data_structs->l->data_head); }
        free(data_structs->l);
    }
    free_symbol_linked_list(data_structs->symbol_head);
    free_data_linked_list(data_structs->data_head);
    free_ext_ent_linked_list(data_structs->ent_head);
    free_ext_ent_linked_list(data_structs->ext_head);
    free(data_structs);
}

CMD_PARSER_VAR *allocate_for_parse_fn(){
    CMD_PARSER_VAR *parser_var = calloc(1, sizeof(CMD_PARSER_VAR));
    if (!parser_var) exit(EXIT_FAILURE);

    parser_var->numbers = calloc(MAXLINE, sizeof(long));
    parser_var->label = calloc(MAXPARAM, sizeof(char));
    if (!parser_var->numbers || !parser_var->label){
        exit(EXIT_FAILURE);
    }
    if (IS_DEBUG) {
        parser_var->code_bin_head = calloc(CODE_LEN, sizeof(char));
        if (!parser_var->code_bin_head) exit(EXIT_FAILURE);
        parser_var->code_bin_str = parser_var->code_bin_head;
    }
    return parser_var;
}

void *create_node(char **param, size_t size, int char_amount) {
    *param = calloc(char_amount, sizeof(char));
    void *new_node = calloc(1, size);
    if (!new_node) {
        exit(EXIT_FAILURE);
    }
    if (!*param){
        exit(EXIT_FAILURE);
    }
    return new_node;
}

symbol_node *create_symbol_node() {
    char *param;
    symbol_node *new_node = create_node(&param, sizeof(symbol_node), MAXPARAM);
    new_node->name = param;
    new_node->vis = NO_VIS, new_node->attr = NO_ATTR;
    return new_node;
}

ext_ent_node *create_ext_ent_node() {
    char *param;
    ext_ent_node *new_node = create_node(&param, sizeof(ext_ent_node), MAXPARAM);
    new_node->name = param;
    return new_node;
}

data_node *create_data_node() {
    char *param;
    data_node *new_node = create_node(&param, sizeof(data_node), MAXLINE);
    new_node->original_ins = param;
    return new_node;
}

void init_hex(DATA_STRUCTS *data_structs){
    struct HEX *hex = calloc(1, sizeof (struct HEX));
    if (hex) {
        data_structs->hex = hex;
        data_structs->hex->whole = 0;
        data_structs->hex->bit_num = 31;
    } else {
        exit(EXIT_FAILURE);
    }
}

void init_line(DATA_STRUCTS *data_structs){
    LINE *l = calloc(1, sizeof(LINE));
    if (l) {
        data_structs->l = l;
        data_structs->l->data_head = calloc(MAXLINE, sizeof(char));
        if (data_structs->l->data_head) {
            data_structs->l->data = data_structs->l->data_head,
            data_structs->l->line_num = 0, data_structs->l->len = 0,
            data_structs->l->IC = 100, data_structs->l->DC = 0,
            data_structs->l->is_error = FALSE;
        } else {
            exit(EXIT_FAILURE);
        }
    }
}

void free_cmd_parser_var(CMD_PARSER_VAR *parser_var){
    if (!parser_var) return;
    if (!parser_var->code_bin_head) { free(parser_var->code_bin_head); }
    if (!parser_var->numbers) { free(parser_var->numbers); }
    if (!parser_var->label) { free(parser_var->label); }
    free(parser_var);
}

void free_symbol_linked_list(symbol_node *head){
    symbol_node * temp;
    while (head != NULL){
        temp = head;
        head = head->next;
        if (temp->name) free(temp->name);
        free(temp);
    }
}

void free_ext_ent_linked_list(ext_ent_node *head){
    ext_ent_node * temp;
    while (head != NULL){
        temp = head;
        head = head->next;
        if (temp->name) free(temp->name);
        free(temp);
    }
}

void free_data_linked_list(data_node *head){
    data_node * temp;
    while (head->next != NULL){
        temp = head;
        head = head->next;
        if (temp->data) free(temp->data);
        if(temp->original_ins) free(temp->original_ins);
        free(temp);
    }
}

char * remove_extension(char *file_name){
    char *new_file_name = calloc(MAXLINE, sizeof(char));
    if (!new_file_name) exit(EXIT_FAILURE);
    char *ext = strstr(file_name, INPUT_FILE_EXT);
    if (!ext) return NULL;
    int extLen = (int) strlen(INPUT_FILE_EXT);
    if (ext[extLen] != '\0') return NULL;
    strncpy(new_file_name, file_name, strlen(file_name) - extLen);
    return new_file_name;
}
