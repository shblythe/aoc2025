#include <stddef.h>
#include <stdint.h>

/*
 * A struct representing a sized array of int64_t
 */
typedef struct s_int_array {
    size_t length;
    int64_t* elements;
} int_array;

size_t read_file(const char* path, char** buffer);
void free_read_file(char* buffer);
size_t read_file_of_string_list(const char* path, char*** buffer);
void free_read_file_of_string_list(char** buffer, size_t num_lines);
size_t read_file_of_ints(const char* path, int64_t** buffer);
void free_read_file_of_ints(int64_t* buffer);
size_t read_file_of_int_rows(const char* path, int_array** rows);
void free_file_of_int_rows(size_t length, int_array* rows);
size_t parse_ints_in_string(char* string, int64_t** buffer);