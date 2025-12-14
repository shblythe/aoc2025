#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "file.h"

int64_t calc_area_with_vals(int_array *a, int_array *b, int* xvals, int *yvals)
{
    int64_t ax = a->elements[0];
    int64_t bx = b->elements[0];
    int64_t ay = a->elements[1];
    int64_t by = b->elements[1];
    if (xvals)
    {
        ax = xvals[ax];
        bx = xvals[bx];
    }
    if (yvals)
    {
        ay = yvals[ay];
        by = yvals[by];
    }
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

int64_t calc_area(int_array *a, int_array *b)
{
    calc_area_with_vals(a, b, NULL, NULL);
}

int compare_int64s_desc(const void* va, const void* vb)
{
    int64_t a = *(const int64_t*)va;
    int64_t b = *(const int64_t*)vb;
    if (a>b) return -1;
    if (a<b) return 1;
    return 0;
}

int compare_ints(const void* va, const void* vb)
{
    int a = *(const int*)va;
    int b = *(const int*)vb;
    if (a<b) return -1;
    if (a>b) return 1;
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

void find_vals(int_array* pA, int_array* pB, int *xvals, int* yvals,
    // below are outputs
    int64_t* ax, int64_t *ay, int64_t *bx, int64_t *by)
{
    (*ax) = -1;
    (*ay) = -1;
    (*bx) = -1;
    (*by) = -1;
    // printf("Before: a=%ld,%ld b=%ld,%ld\n", pA->elements[0], pA->elements[1], pB->elements[0], pB->elements[1]);
    for (int i=0; xvals[i]; ++i)
    {
        if ((*ax)<0 && xvals[i]==pA->elements[0])
            (*ax)=i;
        if ((*bx)<0 && xvals[i]==pB->elements[0])
            (*bx)=i;
        if ((*ay)<0 && yvals[i]==pA->elements[1])
            (*ay)=i;
        if ((*by)<0 && yvals[i]==pB->elements[1])
            (*by)=i;
    }
    // printf("After: a=%ld,%ld b=%ld,%ld\n", *ax, *ay, *bx, *by);
}

void draw_path(char ***pFloor, int_array* pA, int_array* pB, int* xvals, int* yvals)
{
    int64_t ax;
    int64_t ay;
    int64_t bx;
    int64_t by;
    find_vals(pA, pB, xvals, yvals, &ax, &ay, &bx, &by);
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
    if ((x<0) || (y<0) || (x >= size_x) || (y >= size_y) || ((*pFloor)[x][y] != find))
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

    // We're going to enumerate our x and y values and use the enumerations
    // to reduce the size of the grid we need to create, until we need to know
    // the area.
    // We won't worry about removing duplicates - it doesn't matter that much.
    int *xvals;
    int *yvals;
    xvals = (int*)malloc((num_rows+1)*sizeof(int));
    yvals = (int*)malloc((num_rows+1)*sizeof(int));
    for (int i=0; i<num_rows; i++)
    {
        xvals[i] = int_rows[i].elements[0];
        yvals[i] = int_rows[i].elements[1];
    }
    xvals[num_rows]=0; // Null-terminate - these won't get sorted
    yvals[num_rows]=0;
    qsort(xvals, num_rows, sizeof(int), compare_ints);
    qsort(yvals, num_rows, sizeof(int), compare_ints);

    int max_x = num_rows-1;
    int max_y = num_rows-1;
    int size_x = max_x + 1;
    int size_y = max_y + 1;

    // for (int i=0; i<num_rows; i++)
    //     printf("xvals[%d]=%d\n", i, xvals[i]);
    // for (int i=0; i<num_rows; i++)
    //     printf("yvals[%d]=%d\n", i, yvals[i]);
    // printf("Creating 2-d array %d x %d\n", size_x, size_y);
    // Create a 2-d array for the floor plan
    char** floor = (char**)malloc((size_x)*sizeof(char*));
    for (int i=0; i<size_x; i++)
    {
        floor[i] = (char*)malloc(size_y*sizeof(char));
        for (int j=0; j<size_y; j++)
            floor[i][j] = '.';
    }
    // print_floor(floor, size_x, size_y);

    // printf("Drawing paths\n");
    // Draw the initial path of red and green tiles
    for (int i=1; i<num_rows; i++)
    {
        draw_path(&floor, &int_rows[i-1], &int_rows[i], xvals, yvals);
    }
    draw_path(&floor, &int_rows[num_rows-1], &int_rows[0], xvals, yvals);
    // print_floor(floor, size_x, size_y);

    // printf("Finding inside coordinate\n");
    // Find a coordinate which is definitely inside the enclosed area,
    // and not on the boundary, from which to start the fill.
    // Check each point on the floor until we find one.
    int inside_x = -1;
    int inside_y = -1;
    for (int y=0; y<size_y && inside_x<0; y++)
    {
        int x;
        // Skip to the first space after an edge
        for (x=0; (x<size_x-2) && (floor[x][y]!='X'); x++)
            ;
        x++;
        for (; x<size_x; x++)
        {
            // printf("%d %d\n", x, y);
            // Ignore points on edges and vertices
            if (floor[x][y] != '.')
                continue;
            // printf("Checking %d %d\n", x, y);
            // Cast a ray right, and count the number of boundary crossings.
            bool ignore = false;
            int edge_count = 0;
            for (int check_x=x; check_x<size_x; check_x++)
            {
                if (floor[check_x][y] != '.')
                {
                    // printf("x,y = %d,%d\n", check_x, y);
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
            // if (edge_count > 0)
            //     printf("edge_count = %d ignore=%s\n",edge_count,ignore?"true":"false");
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
    // printf("inside_x=%d inside_y=%d\n", inside_x, inside_y);

    // Fill the enclosed areas
    printf("If you get a segfault the stack overflowed\n");
    printf("Can run with `valgrind --main-stacksize=100000000`\n");
    flood_fill(&floor, size_x, size_y, inside_x, inside_y, '.', 'X');
    // print_floor(floor, size_x, size_y);
    // printf("filled\n");

    // Calculate the pairs and areas with coordinates
    int num_pairs = num_rows*(num_rows-1)/2;
    pair *pairs = (pair*)malloc(num_pairs * sizeof(pair));
    int p=0;
    for (int i=0; i<num_rows; i++)
        for (int j=i+1; j<num_rows; j++, p++)
        {
            pairs[p].a = &int_rows[i];
            pairs[p].b = &int_rows[j];
            pairs[p].area = calc_area_with_vals(&int_rows[i], &int_rows[j], xvals, yvals);
        }

    // printf("calculated pairs\n");
    // Go through them in sorted order of area, and check
    // each only contains red and green tiles (tile by tile).
    qsort(pairs, num_pairs, sizeof(pair), compare_pairs_area_desc);
    // printf("Checking pairs\n");
    for (int i=0; i<num_pairs; i++)
    {
        // printf("%d of %d\n", i, num_pairs);
        // Check if the rectangle represented by this pair is filled
        int64_t ax;
        int64_t ay;
        int64_t bx;
        int64_t by;
        find_vals(pairs[i].a, pairs[i].b, xvals, yvals, &ax, &ay, &bx, &by);
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
    free(pairs);
    free(xvals);
    free(yvals);
    for (int i=0; i<size_x; i++)
        free(floor[i]);
    free(floor);
    free_file_of_int_rows(num_rows, int_rows);
}

int main() {
    part1();
    part2();
    return 0;
}
