#include <stdio.h>
#include "file.h"

int count_and_remove_rolls(char ***pLines, size_t num_lines)
{
    int count = 0;
    char **lines = *pLines;
    for (int i=0; i<num_lines; i++)
    {
        // printf("%d : %s\n", i, lines[i]);
        for (int j=0; lines[i][j]; j++)
        {
            if (lines[i][j] != '@')
                continue;   // No paper roll here
            int neighbours = 0;
            if (i > 0)
            {
                if (j > 0 && (lines[i-1][j-1] == '@' || lines[i-1][j-1] == 'x'))
                {
                    neighbours++;
                }
                if (lines[i-1][j] == '@' || lines[i-1][j] == 'x')
                {
                    neighbours++;
                }
                if (lines[i-1][j+1] == '@' || lines[i-1][j+1] == 'x')
                {
                    neighbours++;
                }
            }
            if (j > 0 && (lines[i][j-1] == '@' || lines[i][j-1] == 'x'))
            {
                neighbours++;
            }
            if (lines[i][j+1] == '@' || lines[i][j+1] == 'x')
            {
                neighbours++;
            }
            if (i < num_lines-1)
            {
                if (j > 0 && (lines[i+1][j-1] == '@' || lines[i+1][j-1] == 'x'))
                {
                    neighbours++;
                }
                if (lines[i+1][j] == '@' || lines[i+1][j] == 'x')
                {
                    neighbours++;
                }
                if (lines[i+1][j+1] == '@' || lines[i+1][j+1] == 'x')
                {
                    neighbours++;
                }
            }
            if (neighbours < 4)
            {
                count++;
                lines[i][j] = 'x';
            }
        }
    }
    for (int i=0; i<num_lines; i++)
    {
        // printf("%d : %s\n", i, lines[i]);
        for (int j=0; lines[i][j]; j++)
        {
            if (lines[i][j] == 'x')
                lines[i][j] = '.';
        }
    }
    return count;
}

void part1()
{
    char **lines;
    size_t num_lines = read_file_of_string_list("days/day4/input.txt", &lines);
    printf("%zu lines\n", num_lines);
    int count = count_and_remove_rolls(&lines, num_lines);
    printf("Part1: %d\n", count);
    free_read_file_of_string_list(lines, num_lines);
}

void part2()
{
    char **lines;
    size_t num_lines = read_file_of_string_list("days/day4/input.txt", &lines);
    printf("%zu lines\n", num_lines);
    int total_count = 0;
    int count;
    do {
        count = count_and_remove_rolls(&lines, num_lines);
        // printf("%d\n", count);
        total_count += count;
    } while (count > 0);
    printf("Part2: %d\n", total_count);
    free_read_file_of_string_list(lines, num_lines);
}

int main() {
    part1();
    part2();
    return 0;
}
