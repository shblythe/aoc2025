#include <stdio.h>
#include <stdlib.h>
#include "file.h"

int main() {
    char *buffer;
    size_t length = read_file("test_input.txt", &buffer);
    printf("File buffer (%d bytes):\n%s\n", (int)length, buffer);
    free_read_file(buffer);

    char **lines;
    size_t num_lines = read_file_of_string_list("test_input.txt", &lines);
    printf("%d lines\n", (int)num_lines);
    for (int i=0; i<num_lines; i++)
        printf("%d : %s\n", i, lines[i]);
    free_read_file_of_string_list(lines, num_lines);

    int64_t *ints;
    size_t num_ints = read_file_of_ints("test_ints.txt", &ints);
    printf("%d ints\n", (int)num_ints);
    for (int i=0; i<num_ints; i++)
        printf("%d : %lld\n", i, (long long)ints[i]);
    free_read_file_of_ints(ints);

    return 0;
}
