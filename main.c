/*
 * This program receives a list of .as files (through the command-line) and,
 * should the code be valid, create a .ob file with the code encoded to hex.
 * In addition, should the code refer to other files, the program would create
 * .ent and .ext files that lists those references.
 * If the code is invalid, the program would print the first error of each line.
 *
 */

#include <stdlib.h>
#include "main.h"
#include "service.h"
#include "encoder.h"
#include "memory_manager.h"
#include "output_manager.h"
#include "error_handler.h"
#include "input_validator.h"

extern INSTRUCTION instruct_array[];
extern const short OPCODE;
extern const short RS_T_D;
extern const short FUNCT;
extern const short IMMED;
extern const char COMMENT_INDICATOR;
extern const char binary_print_format[];
extern const char *saved_words[];


int main(int argc, char *argv[]) {
    if (argc < 2) return 0;
    while (--argc > 0)
        process_file(argv[argc]);
    return 0;
}

void process_file(char *file_name){
    FILE *fp = NULL;
    if ((fp = fopen(file_name, "r")) == NULL) {
       return;
    }
    get_input(fp, file_name);
    fclose(fp);
}

void get_input(FILE *fp, char *file_name) {
    char *param = calloc(MAXPARAM, sizeof(char));
    if (!param) { exit(EXIT_FAILURE); }

    DATA_STRUCTS *data_structs = create_data_structures(file_name);
    if (data_structs == NULL) { free(param); return; }
    LINE *l = data_structs->l;

    /* First pass is a validation pass, inserts any data given by instructions,
    * (.dw, .ascciz, etc.), build a symbol table. */
    read_file(fp, data_structs, param, TRUE);

    /* second pass encodes to hex and preform additional validation that depended on previous pass */
    if (!l->is_error){
        init_encoding_pass(fp, data_structs);
        read_file(fp, data_structs, param, FALSE);
        if (!l->is_error) {
            print_data_to_file(data_structs);
            print_ext_ent_manager(data_structs);
        }
    }
    if (l->is_error) {
        fclose(data_structs->hex_output_file);
        remove(data_structs->hex_file_name);
    }
    free_data_struct(data_structs);
    free(param);
}

void read_file(FILE *fp, DATA_STRUCTS *data_structs, char *param, BOOL is_first_pass){
    LINE *l = data_structs->l;
    while ((l->len = getline(l->data_head, MAXLINE, fp)) > 0) {
        (l->line_num)++;
        l->data = l->data_head;
        if (l->len > (MAXLINE-2) && l->data[l->len-1] != '\n'){
            error_line_exceed_length(data_structs->input_file_name, l, fp);
            continue;
        }
        skip_non_n_whitespace(&l->data);
        if (*l->data == '\n' || *l->data == COMMENT_INDICATOR) continue;
        if (is_first_pass) {
            first_pass(data_structs, param);
        } else {
            second_pass(data_structs, param);
        }
    }
}

void init_encoding_pass(FILE *fp, DATA_STRUCTS *data_structs){
    int icf, dcf;
    data_structs->l->line_num = 0;
    icf = data_structs->l->IC;
    dcf = data_structs->l->DC;
    update_symbols(data_structs, icf);
    update_data_img(data_structs, icf);
    print_ob_file_header(data_structs, icf, dcf);
    fseek(fp, 0, SEEK_SET);
    data_structs->l->is_error = FALSE;
    data_structs->l->IC = 100;
}

void update_symbols (DATA_STRUCTS *dataStructs, int icf) {
    struct symbol_node *symbol = dataStructs->symbol_head;
    while (symbol != NULL){
        if (symbol->attr == DATA)
            symbol->value += icf;
        symbol = symbol->next;
    }
}

void update_data_img (DATA_STRUCTS *dataStructs, int icf){
    struct data_node *data = dataStructs->data_head;
    while (data){
        data->symbol_value += icf;
        data = data->next;
    }
}
