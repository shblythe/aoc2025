#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

void part1() {
    int64_t *ints;
    size_t num_ints = read_file_of_ints("days/day2/input.txt", &ints);
    printf("%d ints\n", (int)num_ints);
    int64_t sum_invalid = 0;
    int longest = 0;
    for (int i=0; i<num_ints; i+=2)
    {
        int64_t bottom = ints[i];
        int64_t top = -ints[i+1];   // appears -ve due to dash on parsing
        for (int64_t code = bottom; code <= top; code++)
        {
            char code_str[100];
            sprintf(code_str, "%ld", code);
            int len = strlen(code_str);
            if (len > longest)
                longest = len;
            if (len % 2 != 0)
                continue;
            if (strncmp(code_str, code_str + len/2, len/2) != 0)
                continue;
            sum_invalid += code;
        }
    }
    printf("Part1: %ld\n", sum_invalid);
    printf("Longest: %d\n", longest);
    free_read_file_of_ints(ints);
}

// A bit of a cheat.
// I worked out that any invalid number will be divisible by
// 11..1 - where that's the same length as the number
// and also 00..100..100..1... where each of those "00..1" patterns
// is the same length as one of the number's length's factors.
// (Obviously omitted leading 0s from literals below, or they'd be octal!)
//
// Could have worked these out in the code, but got the max length from
// part 1 instead and worked these out by hand.  Probably quicker.
const int64_t factors[][20] =
{
    /*  0 */ {0},
    /*  1 */ {0},
    /*  2 */ {11, 0},
    /*  3 */ {111, 0},
    /*  4 */ {1111, 101, 0},
    /*  5 */ {11111, 0},
    /*  6 */ {111111, 10101, 1001, 0},
    /*  7 */ {1111111, 0},
    /*  8 */ {11111111, 1010101, 10001, 0},
    /*  9 */ {111111111, 1001001, 0},
    /* 10 */ {1111111111, 101010101, 100001, 0},
};

void part2() {
    int64_t *ints;
    size_t num_ints = read_file_of_ints("days/day2/input.txt", &ints);
    printf("%d ints\n", (int)num_ints);
    int64_t sum_invalid = 0;
    for (int i=0; i<num_ints; i+=2)
    {
        int64_t bottom = ints[i];
        int64_t top = -ints[i+1];   // appears -ve due to dash on parsing
        for (int64_t code = bottom; code <= top; code++)
        {
            char code_str[20];
            sprintf(code_str, "%ld", code);
            int len = strlen(code_str);
            bool valid = true;
            for (int i = 0; factors[len][i] && valid; i++)
            {
                if ((code % factors[len][i]) == 0)
                {
                    valid = false;
                }
            }
            if (!valid)
                sum_invalid += code;
        }
    }
    printf("Part2: %ld\n", sum_invalid);
    free_read_file_of_ints(ints);
}

int main() {
    part1();
    part2();
    return 0;
}
