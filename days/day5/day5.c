#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "file.h"

void part1()
{
    char **lines;
    const char FILE[] = "days/day5/input.txt";
    size_t num_lines = read_file_of_string_list(FILE, &lines);
    printf("%zu lines\n", num_lines);
    int blank;
    for (int i=0; i<num_lines; i++)
    {
        // printf("%d : %s\n", i, lines[i]);
        if (lines[i][0] == 0) {
            blank = i;
        }
    }
    // printf("blank at %d\n", blank);
    free_read_file_of_string_list(lines, num_lines);

    int_array *int_rows;
    size_t num_rows = read_file_of_int_rows(FILE, &int_rows);
    // printf("%zu rows\n", num_rows);
    int fresh = 0;
    for (int i=blank+1; i<num_rows; i++)
    {
        int64_t ingredient = int_rows[i].elements[0];
        // printf("%ld\n", int_rows[i].elements[0]);
        for (int j=0; j<blank; j++)
        {
            if (ingredient >= int_rows[j].elements[0] && ingredient <= -int_rows[j].elements[1]) {
                fresh++;
                break;
            }
        }
    }
    printf("Part1: %d\n", fresh);
    free_file_of_int_rows(num_rows, int_rows);
}

int compare_rows(const void* a, const void *b)
{
    if (((int_array*)a)->elements[0] == ((int_array*)b)->elements[0])
        return 0;
    if (((int_array*)a)->elements[0] > ((int_array*)b)->elements[0])
        return 1;
    return -1;
}

void part2()
{
    char **lines;
    const char FILE[] = "days/day5/input.txt";
    size_t num_lines = read_file_of_string_list(FILE, &lines);
    printf("%zu lines\n", num_lines);
    int blank;
    for (int i=0; i<num_lines; i++)
    {
        // printf("%d : %s\n", i, lines[i]);
        if (lines[i][0] == 0) {
            blank = i;
        }
    }
    // printf("blank at %d\n", blank);
    free_read_file_of_string_list(lines, num_lines);

    int_array *int_rows;
    size_t num_rows = read_file_of_int_rows(FILE, &int_rows);
    // printf("%zu rows\n", num_rows);
    qsort(int_rows, blank, sizeof(int_array), compare_rows);
    int64_t count=0;
    int64_t last_max=0;
    for (int i=0; i<blank; i++)
    {
        int64_t range_min = int_rows[i].elements[0];
        int64_t range_max = -int_rows[i].elements[1];
        if (i>0 && range_min <= last_max)
            range_min = last_max + 1;
        if (range_min > range_max)
            continue;
        // printf("Processing range: %ld - %ld\n", range_min, range_max);
        count += (range_max - range_min) + 1;
        last_max = range_max;
    }
    printf("Part2: %ld\n", count);
    free_file_of_int_rows(num_rows, int_rows);
}

int main() {
    part1();
    part2();
    return 0;
}
