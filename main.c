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

extern Instruction instruct_array[];
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
        printf("File %s was not found or is restricted.\n", file_name);
       return;
    }
    get_input(fp, file_name);
    fclose(fp);
}

void get_input(FILE *fp, char *file_name) {
    Data_structs *data_structs = NULL;
    Line *l = NULL;
    char *param = calloc(MAXPARAM, sizeof(char));
    if (!param) { exit(EXIT_FAILURE); }

    data_structs = create_data_structures(file_name);
    if(!data_structs) return;
    l = data_structs->line;

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

void read_file(FILE *fp, Data_structs *data_structs, char *param, BOOL is_first_pass){
    Line *l = data_structs->line;
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

void init_encoding_pass(FILE *fp, Data_structs *data_structs){
    int icf, dcf;
    data_structs->line->line_num = 0;
    icf = data_structs->line->IC;
    dcf = data_structs->line->DC;
    update_symbols(data_structs, icf);
    update_data_img(data_structs, icf);
    print_ob_file_header(data_structs, icf, dcf);
    fseek(fp, 0, SEEK_SET);
    data_structs->line->is_error = FALSE;
    data_structs->line->IC = 100;
}

void update_symbols (Data_structs *dataStructs, int icf) {
    Symbol_node *symbol = dataStructs->symbol_head;
    while (symbol != NULL){
        if (symbol->attr == DATA)
            symbol->value += icf;
        symbol = symbol->next;
    }
}

void update_data_img (Data_structs *dataStructs, int icf){
    Data_node *data = dataStructs->data_head;
    while (data){
        data->symbol_value += icf;
        data = data->next;
    }
}
