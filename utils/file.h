#include <stddef.h>
#include <stdint.h>
size_t read_file(const char* path, char** buffer);
void free_read_file(char* buffer);
size_t read_file_of_string_list(const char* path, char*** buffer);
void free_read_file_of_string_list(char** buffer, size_t num_lines);
size_t read_file_of_ints(const char* path, int64_t** buffer);
void free_read_file_of_ints(int64_t* buffer);