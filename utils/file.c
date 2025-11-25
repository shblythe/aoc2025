#include "file.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/*
 * Reads a file, puts contents in buffer and returns length
 */
size_t read_file(const char* path, char** buffer)
{
    size_t length;
    FILE* file = fopen(path, "rb");
    if (!file) {
        *buffer = NULL;
        return 0;
    }

    fseek(file, 0, SEEK_END);
    length = ftell(file);
    fseek(file, 0, SEEK_SET);

    *buffer = (char*)malloc(length + 1);
    fread(*buffer, 1, length, file);
    (*buffer)[length] = '\0';

    fclose(file);

    return length;
}

void free_read_file(char* buffer)
{
    free(buffer);
}

/*
 * Replace all occurrences of '\n' in a string with '\0'.
 * Return how many replacements were made.
 */
int str_replace_cr_0(char* buffer)
{
    int count = 0;
    while (*buffer) {
        if (*buffer == '\n')
        {
            *buffer = '\0';
            ++count;
        }
        ++buffer;
    }
    return count;
}

/*
 * Reads a file, creates an array of char* each of which points to a
 * string containing the line read from that file.
 * Returns the number of lines read.
 */
size_t read_file_of_string_list(const char* path, char*** buffer)
{
    char* file_contents;
    size_t num_lines;
    size_t file_length = read_file(path, &file_contents);
    num_lines = str_replace_cr_0(file_contents) + 1;
    *buffer = (char**)malloc(num_lines*sizeof(char**));
    char* contents=file_contents;
    for(int i=0; i<num_lines; i++)
    {
        size_t len = strlen(contents)+1;
        (*buffer)[i] = malloc(len);
        strcpy((*buffer)[i], contents);
        contents += len;
    }
    free_read_file(file_contents);
    return num_lines;
}

void free_read_file_of_string_list(char** buffer, size_t num_lines)
{
    for(int i=0; i<num_lines; i++)
    {
        free(buffer[i]);
    }
    free(buffer);
}

/*
 * Parses all the int64_t in a string, creates an array of them.
 * Any non-ints in the string are ignored.
 * Returns the number of ints parsed.
 * If buffer is NULL on input, it will be allocated, just count.
 */
size_t parse_ints_in_string(char* string, int64_t** buffer)
{
    size_t count = 0;
    while (*string)
    {
        while (*string && ((*string < '0') || (*string > '9')) && (*string != '-') )
            string++;
        if (*string)
        {
            char* endptr;
            int64_t value = strtoll(string, &endptr, 10);
            if (buffer)
            {
                (*buffer)[count] = value;
            }
            count++;
            string = endptr;
        }
    }
    return count;
}

/*
 * Reads a file, creates an array of all the int64_t in the file.
 * Any non-ints in the file are ignored.
 * Returns the number of ints read.
 */
size_t read_file_of_ints(const char* path, int64_t** buffer)
{
    char* file_contents;
    size_t file_length = read_file(path, &file_contents);
    size_t num_ints = parse_ints_in_string(file_contents, NULL);
    *buffer = (int64_t*)malloc(num_ints * sizeof(int64_t));
    parse_ints_in_string(file_contents, buffer);
    free_read_file(file_contents);
    return num_ints;
}

void free_read_file_of_ints(int64_t* buffer)
{
    free(buffer);
}