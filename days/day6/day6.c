#include <stdio.h>
#include <string.h>
#include "file.h"

void part1()
{
    const char FILE[] = "days/day6/input.txt";
    char **lines;
    size_t num_lines = read_file_of_string_list(FILE, &lines);
    printf("%zu lines\n", num_lines);

    int_array *int_rows;
    size_t num_rows = read_file_of_int_rows(FILE, &int_rows);
    int col=0;
    int64_t total=0;
    for(char *op = lines[num_lines-1]; *op; ++op)
    {
        if (*op=='+')
        {
            int64_t sum = 0;
            for (int row=0; row<num_rows-1; ++row)
                sum += int_rows[row].elements[col];
            // printf("+ %ld\n", sum);
            total+=sum;
            ++col;
        }
        if (*op=='*')
        {
            int64_t product = 1;
            for (int row=0; row<num_rows-1; ++row)
                product *= int_rows[row].elements[col];
            // printf("* %ld\n", product);
            total+=product;
            ++col;
        }
    }
    printf("Part1: %ld\n", total);
    free_read_file_of_string_list(lines, num_lines);
    free_file_of_int_rows(num_rows, int_rows);
}

void part2()
{
    const char FILE[] = "days/day6/input.txt";
    char **lines;
    size_t num_lines = read_file_of_string_list(FILE, &lines);
    printf("%zu lines\n", num_lines);
    size_t max_len = 0;
    for (int i=0; i<num_lines; i++)
    {
        // printf("%d : %s\n", i, lines[i]);
        size_t len = strlen(lines[i]);
        if (len > max_len)
            max_len = len;
    }
    for (int i=0; i<num_lines; i++)
    {
        while (strlen(lines[i]) < max_len)
            strcat(lines[i], " ");
    }
    char op = '\0';
    int64_t total = 0;
    int64_t current = 0;
    for (int col=0; ; ++col)
    {
        if (lines[num_lines-1][col] != ' ')
        {
            // Add on the last one
            total += current;
            //printf("current %ld total %ld\n", current, total);
            if (lines[num_lines-1][col] == '\0')
                break; // we're done!
            // Move on to the next one
            op = lines[num_lines-1][col];
            if (op == '*')
                current = 1;
            else
                current = 0;
        }
        int operand = 0;
        for (int row=0; row<num_lines-1; ++row)
        {
            if (lines[row][col] != ' ')
            {
                int value = lines[row][col]-'0';
                operand *= 10;
                operand += value;
            }
        }
        if (operand != 0)
        {
            if (op == '*')
                current *= operand;
            else
                current += operand;
        }
        // printf("operand %d current %ld\n", operand, current);
    }
    printf("Part2: %ld\n", total);
    free_read_file_of_string_list(lines, num_lines);
}

int main() {
    part1();
    part2();
    return 0;
}
