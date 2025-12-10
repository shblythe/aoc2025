#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "file.h"

int64_t calc_area(int_array *a, int_array *b)
{
    // Add one, because coordinates are inclusive
    int64_t x = abs(a->elements[0] - b->elements[0]) + 1;
    int64_t y = abs(a->elements[1] - b->elements[1]) + 1;
    // printf("[%ld, %ld] [%ld, %ld] x=%ld y=%ld a=%ld\n",
    //         a->elements[0], a->elements[1],
    //         b->elements[0], b->elements[1],
    //         x, y, x*y
    // );
    return x*y;
}

int compare_int64s_desc(const void* va, const void* vb)
{
    int64_t a = *(const int64_t*)va;
    int64_t b = *(const int64_t*)vb;
    if (a>b) return -1;
    if (a<b) return 1;
    return 0;
}

void part1()
{
    int_array *int_rows;
    size_t num_rows = read_file_of_int_rows("days/day9/input.txt", &int_rows);
    printf("%zu rows\n", num_rows);
    // for (int i=0; i<num_rows; i++)
    // {
    //     printf("%d (%d) : ", i, (int)int_rows[i].length);
    //     for (int j=0; j<int_rows[i].length; j++)
    //     {
    //         printf("%ld ", int_rows[i].elements[j]);
    //     }
    //     printf("\n");
    // }
    int num_pairs = num_rows*(num_rows-1)/2;
    int64_t *areas = (int64_t*)malloc(num_pairs * sizeof(int64_t));
    int p=0;
    for (int i=0; i<num_rows; i++)
        for (int j=i+1; j<num_rows; j++, p++)
            areas[p] = calc_area(&int_rows[i], &int_rows[j]);
    qsort(areas, num_pairs, sizeof(int64_t), compare_int64s_desc);
    printf("Part1: %ld\n", areas[0]);
    free(areas);
    free_file_of_int_rows(num_rows, int_rows);
}

void print_floor(char** floor, int size_x, int size_y)
{
    for (int y=0; y<size_y; y++)
    {
        for (int x=0; x<size_x; x++)
            printf("%c", floor[x][y]);
        printf("\n");
    }
    printf("\n");
}

void draw_path(char ***pFloor, int_array* pA, int_array* pB)
{
    int64_t ax = pA->elements[0];
    int64_t ay = pA->elements[1];
    int64_t bx = pB->elements[0];
    int64_t by = pB->elements[1];
    int step_x = compare_int64s_desc((const void*)&ax, (const void*)&bx);
    int step_y = compare_int64s_desc((const void*)&ay, (const void*)&by);
    if (step_x == 0)
        step_x = 1;
    if (step_y == 0)
        step_y = 1;
    for (int x=ax; (x*step_x)<=(bx*step_x); x+=step_x)
        for (int y=ay; (y*step_y)<=(by*step_y); y+=step_y)
            (*pFloor)[x][y] = 'X';
}

void flood_fill(char ***pFloor, int size_x, int size_y, int x, int y, char find, char replace)
{
    if (x > size_x || y > size_y || (*pFloor)[x][y] != find)
        return;
    (*pFloor)[x][y] = replace;
    flood_fill(pFloor, size_x, size_y, x-1, y, find, replace);
    flood_fill(pFloor, size_x, size_y, x+1, y, find, replace);
    flood_fill(pFloor, size_x, size_y, x, y-1, find, replace);
    flood_fill(pFloor, size_x, size_y, x, y+1, find, replace);
}

typedef struct pair_s {
    int_array *a;
    int_array *b;
    int64_t area;
} pair;

int compare_pairs_area_desc(const void* va, const void* vb)
{
    pair* a = (pair*)va;
    pair* b = (pair*)vb;
    if (a->area > b->area) return -1;
    if (a->area < b->area ) return 1;
    return 0;
}

void part2()
{
    int_array *int_rows;
    size_t num_rows = read_file_of_int_rows("days/day9/input.txt", &int_rows);
    printf("%zu rows\n", num_rows);

    // Probably need to
    // Find max X and Y
    int max_x = 0;
    int max_y = 0;
    for (int i=0; i<num_rows; i++)
    {
        if (int_rows[i].elements[0] > max_x)
            max_x = int_rows[i].elements[0];
        if (int_rows[i].elements[1] > max_y)
            max_y = int_rows[i].elements[1];
    }
    int size_x = max_x + 1;
    int size_y = max_y + 1;

    printf("Creating 2-d array %d x %d\n", size_x, size_y);
    // Create a 2-d array for the floor plan
    char** floor = (char**)malloc((size_x)*sizeof(char*));
    for (int i=0; i<size_x; i++)
    {
        floor[i] = (char*)malloc(size_y*sizeof(char));
        for (int j=0; j<size_y; j++)
            floor[i][j] = '.';
    }
    // print_floor(floor, size_x, size_y);

    printf("Drawing paths\n");
    // Draw the initial path of red and green tiles
    for (int i=1; i<num_rows; i++)
    {
        draw_path(&floor, &int_rows[i-1], &int_rows[i]);
    }
    draw_path(&floor, &int_rows[num_rows-1], &int_rows[0]);
    // print_floor(floor, size_x, size_y);

    printf("Finding inside coordinate\n");
    // Find a coordinate which is definitely inside the enclosed area,
    // and not on the boundary, from which to start the fill.
    // Check each point on the floor until we find one.
    int inside_x = -1;
    int inside_y = -1;
    for (int y=0; y<size_y && inside_x<0; y++)
    {
        printf("y=%d\n", y);
        int x;
        // Skip to the first space after an edge
        for (int x=0; x<size_x && floor[x][y]!='X'; x++)
            ;
        x++;
        printf("x=%d\n", x);
        for (; x<size_x; x++)
        {
            // printf("%d %d\n", x, y);
            // Ignore points on edges and vertices
            if (floor[x][y] != '.')
                continue;
            printf("Checking %d %d\n", x, y);
            // Cast a ray right, and count the number of boundary crossings.
            bool ignore = false;
            int edge_count = 0;
            for (int check_x=x; check_x<size_x; check_x++)
            {
                if (floor[check_x][y] != '.')
                {
                    if (check_x > x && floor[check_x-1][y] != '.')
                    {
                        // printf("two in a row at %d\n", check_x-1);
                        // If we get two edge spaces in a row, don't use this space, because
                        // we don't want to worry about what happens when we go along an edge.
                        ignore = true;
                        break;
                    }
                    edge_count ++;
                }
            }
            // printf("edge_count = %d ignore=%s\n",edge_count,ignore?"true":"false");
            if (!ignore && (edge_count % 2 == 1))
            {
                inside_x = x;
                inside_y = y;
                break;
            }
        }
    }
    if (inside_x == -1)
    {
        printf("No inside space found!\n");
        exit(1);
    }
    printf("inside_x=%d inside_y=%d\n", inside_x, inside_y);

    // Fill the enclosed areas
    flood_fill(&floor, size_x, size_y, inside_x, inside_y, '.', 'X');
    // print_floor(floor, size_x, size_y);

    // Calculate the pairs and areas with coordinates
    int num_pairs = num_rows*(num_rows-1)/2;
    pair *pairs = (pair*)malloc(num_pairs * sizeof(pair));
    int p=0;
    for (int i=0; i<num_rows; i++)
        for (int j=i+1; j<num_rows; j++, p++)
        {
            pairs[p].a = &int_rows[i];
            pairs[p].b = &int_rows[j];
            pairs[p].area = calc_area(&int_rows[i], &int_rows[j]);
        }

    // Go through them in sorted order of area, and check
    // each only contains red and green tiles (tile by tile).
    qsort(pairs, num_pairs, sizeof(pair), compare_pairs_area_desc);
    printf("Checking pairs\n");
    for (int i=0; i<num_pairs; i++)
    {
        // Check if the rectangle represented by this pair is filled
        int64_t ax = pairs[i].a->elements[0];
        int64_t ay = pairs[i].a->elements[1];
        int64_t bx = pairs[i].b->elements[0];
        int64_t by = pairs[i].b->elements[1];
        int step_x = compare_int64s_desc((const void*)&ax, (const void*)&bx);
        int step_y = compare_int64s_desc((const void*)&ay, (const void*)&by);
        if (step_x == 0)
            step_x = 1;
        if (step_y == 0)
            step_y = 1;
        bool filled = true;
        for (int x=ax; filled && (x*step_x)<=(bx*step_x); x+=step_x)
            for (int y=ay; (y*step_y)<=(by*step_y); y+=step_y)
            {
                if (floor[x][y] == '.')
                {
                    filled = false;
                    break;
                }
            }
        if (filled)
        {
            printf("Part2: %ld\n", pairs[i].area);
            break;
        }
    }
    // Free the floor plan
    for (int i=0; i<size_x; i++)
        free(floor[i]);
    free(floor);

    // Or is there a shortcut?
    // In the example, all the pairs given are 1 or 2 apart
    // in the list - this guarantees that they'll only contain
    // red and green tiles, because they'll create a local
    // rectangle that must be filled with green tiles, but
    // is there any guarantee that they're the only ones?
    // No, could have something like this
    //          ##
    // #XXXXXXXXXXXXXXXXXX#
    // #XXXXXXXXXXXXXXXXXX#
    //          ##
    // It's obvious what the largest rectangle is here, but is doesn't
    // have near-consecutive opposite vertices.
}

int main() {
    part1();
    part2();
    return 0;
}
