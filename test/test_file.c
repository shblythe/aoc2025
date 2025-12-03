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
    printf("%zu lines\n", num_lines);
    for (int i=0; i<num_lines; i++)
        printf("%d : %s\n", i, lines[i]);
    free_read_file_of_string_list(lines, num_lines);

    int64_t *ints;
    size_t num_ints = read_file_of_ints("test_ints.txt", &ints);
    printf("%zu ints\n", num_ints);
    for (int i=0; i<num_ints; i++)
        printf("%d : %lld\n", i, (long long)ints[i]);
    free_read_file_of_ints(ints);

    int_array *int_rows;
    size_t num_rows = read_file_of_int_rows("test_ints.txt", &int_rows);
    printf("%zu rows\n", num_rows);
    for (int i=0; i<num_rows; i++)
    {
        printf("%d (%d) : ", i, (int)int_rows[i].length);
        for (int j=0; j<int_rows[i].length; j++)
        {
            printf("%ld ", int_rows[i].elements[j]);
        }
        printf("\n");
    }
    free_file_of_int_rows(num_rows, int_rows);
    return 0;
}
