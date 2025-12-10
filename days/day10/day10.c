#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

/*
 * A struct representing a sized array of char*
 */
typedef struct s_str_array {
    size_t length;
    char** elements;
} str_array;

/*
 * Replace all occurrences of '\n' in a string with '\0'.
 * Return how many replacements were made.
 */
int str_replace_spc_0(char* buffer)
{
    int count = 0;
    while (*buffer) {
        if (*buffer == ' ')
        {
            *buffer = '\0';
            ++count;
        }
        ++buffer;
    }
    return count;
}

str_array tokenize_string(char* str)
{
    str_array tokens;
    tokens.length = str_replace_spc_0(str) + 1;
    tokens.elements = (char**)malloc(tokens.length * sizeof(char*));
    for(int i=0; i<tokens.length; i++)
    {
        size_t len = strlen(str)+1;
        tokens.elements[i] = malloc(len);
        strcpy(tokens.elements[i], str);
        str += len;
    }
    return tokens;
}

void free_tokenize_string(str_array* arr)
{
    for (int i=0; i<arr->length; i++)
        free(arr->elements[i]);
    free(arr->elements);
}

void part1()
{
    char **lines;
    size_t num_lines = read_file_of_string_list("days/day10/input.txt", &lines);
    printf("%zu lines\n", num_lines);
    int max_fields = 0;
    int total_presses = 0;
    for (int i=0; i<num_lines; i++)
    {
        // printf("%d : %s\n", i, lines[i]);
        str_array tokens = tokenize_string(lines[i]);
        // for (int i=0; i<tokens.length; i++)
        //     printf("%s\n", tokens.elements[i]);
        int target = 0;
        int bit = 1;
        int num_lights = 0;
        for (char* c = tokens.elements[0]+1; *c!=']'; c++, bit<<=1, num_lights++)
        {
            if (*c == '#')
                target |= bit;
        }
        // printf("target %x\n", target);
        int_array buttons;
        buttons.length = tokens.length - 2;
        buttons.elements = (int64_t*)malloc(buttons.length * sizeof(int64_t));
        for (int i=0; i<buttons.length; i++)
        {
            int64_t *lights;
            lights = (int64_t*)malloc(num_lights * sizeof(int64_t));
            memset(lights, -1, num_lights*sizeof(int64_t));
            parse_ints_in_string(tokens.elements[i+1], &lights);
            // printf("%s\n", tokens.elements[i+1]);
            buttons.elements[i] = 0;
            for (int j=0; j<num_lights; j++)
            {
                // printf("%ld\n", lights[j]);
                if (lights[j]>=0)
                    buttons.elements[i] |= (1 << lights[j]);
            }
            // printf("-> %lx\n", buttons.elements[i]);

            free(lights);
        }
        // OK, so now we have the mask of lights affected for each button
        // in buttons array.
        // We need to go through all combinations of buttons.
        int best_result_bits = 32;
        for (int j=0; j<(1 << buttons.length); j++)
        {
            int result = 0;
            // printf("combo: %x\n", j);
            for (int k=0; k<buttons.length; k++)
            {
                if (j & (1 << k))
                {
                    // printf("press %lx\n", buttons.elements[k]);
                    result ^= buttons.elements[k];
                    // printf("current result: %x\n", result);
                }
            }
            if (result == target)
            {
                // printf("Successful combo: %x\n", j);
                int combo = j;
                int bits = 0;
                while (combo)
                {
                    bits += (combo & 1);
                    combo >>= 1;
                }
                if (bits < best_result_bits)
                    best_result_bits = bits;
            }
        }
        total_presses += best_result_bits;

        free(buttons.elements);
        free_tokenize_string(&tokens);
    }
    printf("part1: %d\n", total_presses);
    free_read_file_of_string_list(lines, num_lines);
}

int main() {
    part1();
    return 0;
}
