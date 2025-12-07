#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

void part1()
{
    char **lines;
    size_t num_lines = read_file_of_string_list("days/day7/input.txt", &lines);
    printf("%zu lines\n", num_lines);
    int width = strlen(lines[0]);
    char *curr_row = (char*)malloc(width+1);
    memset(curr_row, '.', width);
    curr_row[width] = '\0';
    for (int i=0; i<width; i++)
    {
        if (lines[0][i] == 'S')
        {
            curr_row[i] = '|';
            break;
        }
    }
    char *next_row = (char*)malloc(width+1);
    next_row[width] = '\0';
    int splits = 0;
    for (int i=1; i<num_lines; i++)
    {
        memset(next_row, '.', width);
        // printf("%2d : %s\n", i, lines[i]);
        for (int j=0; j<width; j++)
        {
            if (curr_row[j] == '|')
            {
                if (lines[i][j] == '^')
                {
                    next_row[j-1] = '|';
                    next_row[j+1] = '|';
                    ++splits;
                }
                else
                    next_row[j] = '|';
            }
        }
        // printf("   : %s\n", next_row);
        memcpy(curr_row, next_row, width);
    }
    printf("Part1: %d\n", splits);

    free_read_file_of_string_list(lines, num_lines);
}

int calcs = 0;

int64_t count_timelines(char **lines, int num_lines, int x, int start_y, int64_t ***cache)
{
    // printf("count_timelines(.., %d, %d, %d)\n", num_lines, x, start_y);
    if (cache && (*cache)[start_y][x])
    {
        // printf("cache hit: %d %d\n", x, start_y);
        return (*cache)[start_y][x];
    }
    ++calcs;
    for(int y=start_y; y<num_lines; y++)
    {
        // printf("%c\n", lines[y][x]);
        if (lines[y][x] == '^')
        {
            int64_t rval =  count_timelines(lines, num_lines, x-1, y+1, cache)
                +   count_timelines(lines, num_lines, x+1, y+1, cache);
            if (cache)
            {
                for (int i=start_y; i<=y; i++)
                (*cache)[i][x] = rval;
            }
            return rval;
        }
    }
    if (cache)
    {
        for (int i=start_y; i<num_lines; i++)
            (*cache)[i][x] = 1;
    }
    return 1;
}

void part2()
{
    char **lines;
    size_t num_lines = read_file_of_string_list("days/day7/input.txt", &lines);
    printf("%zu lines\n", num_lines);
    int width = strlen(lines[0]);
    int start_pos;
    for (int i=0; i<width; i++)
    {
        if (lines[0][i] == 'S')
        {
            start_pos = i;
            break;
        }
    }

    int64_t **cache = (int64_t **)malloc(num_lines*sizeof(int64_t*));
    for (int i=0; i<num_lines; i++)
    {
        cache[i] = (int64_t *)malloc(width*sizeof(int64_t));
        memset(cache[i], 0, width*sizeof(int64_t));
    }
    // {
    //     printf("Baseline\n");
    //     calcs = 0;
    //     int64_t timelines = count_timelines(lines, num_lines, start_pos, 0, NULL);
    //     printf("Part2: %ld\n", timelines);
    //     printf("calcs: %d\n", calcs);
    //     printf("\n");
    // }
    {
        printf("Cached\n");
        calcs = 0;
        int64_t timelines = count_timelines(lines, num_lines, start_pos, 0, &cache);
        printf("Part2: %ld\n", timelines);
        printf("calcs: %d\n", calcs);
    }

    free_read_file_of_string_list(lines, num_lines);
}

int main() {
    part1();
    part2();
    return 0;
}
