//ishitagupta :)
#include "word.h"
#include "unscramble.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int myStrcmp(const char *str1, const char *str2);

int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input file> <output file>\n", argv[0]);
        exit(1);
    }

    FILE *in = fopen(argv[1], "r");
    FILE *out = fopen(argv[2], "w");

    int num_bytes;
    char file_name[20];
    char file_status[6];

    fscanf(in, "%d %s %s", &num_bytes, file_name, file_status);
    FILE *binary_file = fopen(file_name, "rb");

    fprintf(out, "Bytes in file:    %d\n", num_bytes);
    fprintf(out, "Name of file:     %s\n", file_name);
    fprintf(out, "Clear|Fuzzy:      %s\n", file_status);

    uint8_t *encoded_bytes = malloc(num_bytes);
    // size_t fread ( void * ptr, size_t size, size_t count, FILE * stream );
    fread(encoded_bytes, sizeof(uint8_t), num_bytes, binary_file);

    struct Word_List decoded_text;

    // choose unscramble method based on the file status
    if (myStrcmp(file_status, "clear") == 0) {
        clear(encoded_bytes, &decoded_text);
    } 
    else if (myStrcmp(file_status, "fuzzy") == 0) {
        fuzzy(encoded_bytes, &decoded_text);
    } 
    else {
        fprintf(stderr, "Invalid status: %s\n", file_status);
        exit(1);
    }

    display_words(out, decoded_text);

    fclose(in);
    fclose(out);
    fclose(binary_file);

    free(encoded_bytes);
    free_list(decoded_text);

    return 0;
}

int myStrcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(const unsigned char *)str1 - *(const unsigned char *)str2;
}