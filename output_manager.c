#include <stdlib.h>
#include "output_manager.h"
#include "service.h"

const char binary_print_format[] = "%04d%-11s %-40s %-34s\n";
const char ext_ent_format[] = "%s %04d\n";
extern BOOL IS_DEBUG;

void print_ob_file_header(DATA_STRUCTS *data_structs, int icf, int dcf){
    fprintf(data_structs->hex_output_file, "%-4s %d %d", "", (icf - 100), dcf);
}

void print_data_to_file(DATA_STRUCTS *data_structs){
    int i = 0, cells = 4, size;
    PARAM_TYPE param_type;
    struct data_node *data = data_structs->data_head;
    while (data->next){
        param_type = data->param_type;
        size = get_data_size(param_type);
        while (i < data->data_count) {
            switch (param_type) {
                case DB:
                case ASCIZ:
                    print_hex_to_file(data_structs, ((unsigned char *) data->data)[i], size, &cells,
                                      &data_structs->l->IC);
                    break;
                case DH:
                    print_hex_to_file(data_structs, ((int *) data->data)[i], size, &cells, &data_structs->l->IC);
                    break;
                case DW:
                    print_hex_to_file(data_structs, ((long *) data->data)[i], size, &cells, &data_structs->l->IC);
                    break;
            }
            i++;
        }
        data = data->next;
        i=0;
    }
}

void print_ext_ent_manager(DATA_STRUCTS *data_structs) {
    char *ext_name = add_extension_to_file(data_structs->input_file_name, ".ext"); /*allocation safe*/
    char *ent_name = add_extension_to_file(data_structs->input_file_name, ".ent"); /*allocation safe*/
    print_ext_ent_to_file(data_structs->ext_head, ext_name);
    print_ext_ent_to_file(data_structs->ent_head, ent_name);
}

void print_ext_ent_to_file(ext_ent_node *head, char *file_name){
    ext_ent_node *cur = head;
    FILE *fp = NULL;
    if (head->next != NULL)
        fp = fopen(file_name, "w+");
    if (fp) {
        while (cur->next != NULL){
            fprintf(fp, ext_ent_format, cur->name, cur->value);
            cur = cur->next;
        }
        fclose(fp);
    }
    free(file_name);

}

void print_hex_to_file (DATA_STRUCTS *data_structs, unsigned long num, int bits_num, int *cells, int *line_num){
    while(bits_num-- > 0) {
        if (*cells == 4) {
            fprintf(data_structs->hex_output_file, "\n%04d ", *line_num);
            *cells = 0;
            (*line_num) += 4;
        }
        fprintf(data_structs->hex_output_file, "%02lx ", num & 0xff);
        (*cells)++;
        num >>= 8;
    }
}

void encode_to_hex(struct HEX *hex, int no_bits, unsigned long to_encode){
    int j = 0;
    unsigned long bit;
    unsigned long max_pow = 1 << (no_bits-1);
    for (; j < no_bits; ++j){
        bit = to_encode & max_pow;
        if (bit!=0)
            hex->whole |= 1 << hex->bit_num;
        hex->bit_num--;
        to_encode = to_encode << 1;
    }
}

void print_binary_to_stdout (DATA_STRUCTS *data_structs, CMD_PARSER_VAR *parser_var){
    if (!IS_DEBUG) return;
    data_structs->l->data_head[data_structs->l->len-1] = '\0';
    printf(binary_print_format, data_structs->l->IC, "", data_structs->l->data_head,
           parser_var->code_bin_head);
}

void encode_to_bin(char **encode_dest, int no_bits, unsigned int to_encode){
    int j = 0;
    unsigned int max_pow = 1 << (no_bits-1); /* todo how come there's no error, when address is 25 bits and unsigned is 16??? */
    if (!IS_DEBUG) return;
    for (; j < no_bits; ++j){
        *((*encode_dest)++) = to_encode & max_pow ? '1' : '0';
        to_encode = to_encode << 1;
    }
     *((*encode_dest)++) = ' ';
}
