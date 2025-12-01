#include <stdio.h>
#include <stdlib.h>
#include "file.h"

void part1() {
    char **lines;
    size_t num_lines = read_file_of_string_list("days/day1/input.txt", &lines);
    int pos = 50;
    int count = 0;
    for (int i=0; i<num_lines; i++) {
        if (lines[i][0] == 'L')
            lines[i][0] = '-';
        else
            lines[i][0] = ' ';
        int step;
        sscanf(lines[i], "%d", &step);
        pos = (pos + step + 100) % 100;
        if (pos == 0)
            count++;
    }
    printf("Part1: %d\n", count);
    free_read_file_of_string_list(lines, num_lines);
}

void part2() {
    char **lines;
    size_t num_lines = read_file_of_string_list("days/day1/input.txt", &lines);
    int pos = 50;
    int count = 0;
    for (int i=0; i<num_lines; i++) {
        if (lines[i][0] == 'L')
            lines[i][0] = '-';
        else
            lines[i][0] = ' ';
        int step;
        sscanf(lines[i], "%d", &step);
        int start_pos = pos;
        pos += step;
        int clocked = 0;
        while (pos >= 100) {
            pos -= 100;
            count ++;
            // indicate that if we landed on 0 going right
            // we've already counted that.
            clocked = 1;
        }
        while (pos < 0) {
            // Don't count a correction from within -100 if we started on 0!
            if (start_pos != 0 || pos < -100)
                count ++;
            pos += 100;
        }
        if (pos == 0 && !clocked)
            count ++;
    }
    printf("Part2: %d\n", count);
    free_read_file_of_string_list(lines, num_lines);
}

int main() {
    part1();
    part2();
    return 0;
}
