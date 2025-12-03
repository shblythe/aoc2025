#include <stdio.h>
#include <string.h>
#include "file.h"

void part1()
{
    char **lines;
    size_t num_lines = read_file_of_string_list("days/day3/input.txt", &lines);
    printf("%zu lines\n", num_lines);
    int joltage = 0;
    for (int i=0; i<num_lines; i++)
    {
        int len = strlen(lines[i]);
        int digit1 = -1;
        int digit1_pos;
        int digit2 = -1;
        for (int j=0; j<len-1; j++)
        {
            if (lines[i][j] > digit1)
            {
                digit1 = lines[i][j];
                digit1_pos = j;
            }
        }
        for (int j=digit1_pos + 1; j<len; j++)
        {
            if (lines[i][j] > digit2)
                digit2 = lines[i][j];
        }
        int battery = (digit1 - '0') * 10 + digit2 - '0';
        joltage += battery;
    }
    printf("Part1: %d\n", joltage);
    free_read_file_of_string_list(lines, num_lines);
}

int64_t pow_10[] = {
    1,
    10,
    100,
    1000,
    10000,
    100000,
    1000000,
    10000000,
    100000000,
    1000000000,
    10000000000,
    100000000000,
    1000000000000,
};

void part2()
{
    char **lines;
    size_t num_lines = read_file_of_string_list("days/day3/input.txt", &lines);
    printf("%zu lines\n", num_lines);
    int64_t joltage = 0;
    for (int i=0; i<num_lines; i++)
    {
        int len = strlen(lines[i]);
        int digits[12] = { 0 };
        int digits_pos[12];
        int64_t battery = 0;
        for (int digit = 0; digit < 12; digit++)
        {
            int start = (digit == 0) ? 0 : (digits_pos[digit-1] + 1);
            for (int j=start; j<len-(11-digit); j++)
            {
                if (lines[i][j] > digits[digit])
                {
                    digits[digit] = lines[i][j];
                    digits_pos[digit] = j;
                }
            }
            int64_t digit_val = (digits[digit] - '0') * pow_10[11-digit];
            battery += digit_val;
        }
        joltage += battery;
    }
    printf("Part2: %ld\n", joltage);
    free_read_file_of_string_list(lines, num_lines);
}

int main() {
    part1();
    part2();
    return 0;
}
