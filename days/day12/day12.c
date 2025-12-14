#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

#define NUM_SHAPES 6

// Each shape is an array of 2-d maps
// We'll use true and false to represent presence and absence in each "pixel"
// orientations[0] is rotated through 0 deg
// orientations[1] is rotated through 90 deg
// etc.
#define NUM_ORIENTATIONS 4
typedef struct shape_s
{
    bool orientations[NUM_ORIENTATIONS][3][3];
    bool duplicate_orientations[NUM_ORIENTATIONS];
    int pixels;
} shape;

shape new_shape(char** lines)
{
    shape s;
    s.pixels=0;
    for (int i=0; i<3; i++)
    {
        for (int j=0; j<3; j++)
        {
            bool pixel = (lines[i][j]=='#');
            s.orientations[0][i][j] = pixel;
            s.orientations[1][2-j][i] = pixel;
            s.orientations[2][2-i][2-j] = pixel;
            s.orientations[3][j][2-i] = pixel;
            if (pixel)
                s.pixels++;
        }
    }
    for (int i=0; i<NUM_ORIENTATIONS; i++)
        s.duplicate_orientations[i]=false;
    for (int i=0; i<NUM_ORIENTATIONS-1; i++)
    {
        for (int j=i+1; j<NUM_ORIENTATIONS; j++)
        {
            if (s.duplicate_orientations[j])
                continue;
            bool dup = true;
            for (int x=0; dup && (x<3); x++)
                for (int y=0; dup && (y<3); y++)
                    if (s.orientations[i][x][y] != s.orientations[j][x][y])
                    {
                        // printf("%d %d %d %d %c %c\n",
                        //     i, j, x, y,
                        //     s.orientations[i][x][y]?'#':'.',
                        //     s.orientations[j][x][y]?'#':'.'
                        // );
                        dup = false;
                    }
            // printf("%d dup: %s\n", j, dup?"true":"false");
            s.duplicate_orientations[j]=dup;
        }
    }
    return s;
}

void print_shape(shape *s)
{
    for (int o=0; o<NUM_ORIENTATIONS; o++)
    {
        if (s->duplicate_orientations[o])
            continue;
        for (int i=0; i<3; i++)
        {
            for (int j=0; j<3; j++)
            {
                printf("%c", s->orientations[o][i][j]?'#':'.');
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("---\n");
}

typedef struct grid_s
{
    int x_size;
    int y_size;
    int shape_count[NUM_SHAPES];
    bool **current;
} grid;

grid new_grid(char* line)
{
    int64_t *ints;
    size_t num_ints = parse_ints_in_string(line, NULL);
    ints = (int64_t*)malloc(num_ints * sizeof(int64_t));
    parse_ints_in_string(line, &ints);
    grid g;
    g.x_size = ints[0];
    g.y_size = ints[1];
    for (int i=0; i<NUM_SHAPES; i++)
        g.shape_count[i]=ints[2+i];
    free(ints);
    g.current=(bool**)malloc(g.x_size*sizeof(bool*));
    for (int i=0; i<g.x_size; i++)
    {
        g.current[i]=(bool*)malloc(g.y_size*sizeof(bool));
        for (int j=0; j<g.y_size; j++)
            g.current[i][j]=false;
    }
    return g;
}

grid copy_grid(grid *g)
{
    grid ng;
    ng.x_size = g->x_size;
    ng.y_size = g->y_size;
    for (int i=0; i<NUM_SHAPES; i++)
        ng.shape_count[i]=g->shape_count[i];
    ng.current=(bool**)malloc(ng.x_size*sizeof(bool*));
    for (int i=0; i<ng.x_size; i++)
    {
        ng.current[i]=(bool*)malloc(ng.y_size*sizeof(bool));
        for (int j=0; j<ng.y_size; j++)
            ng.current[i][j]=g->current[i][j];
    }
    return ng;
}

void free_grid(grid *g)
{
    for (int i=0; i<g->x_size; i++)
        free(g->current[i]);
    free(g->current);
}

bool fit_shape_in_grid(grid* g, shape* s, int orientation, int x, int y)
{
    if (s->duplicate_orientations[orientation])
        return false;
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            if (g->current[x+i][y+j] && s->orientations[orientation][i][j])
                return false;
    for (int i=0; i<3; i++)
        for (int j=0; j<3; j++)
            g->current[x+i][y+j] = s->orientations[orientation][i][j];
    return true;
}

void print_grid(grid* g)
{
    printf("%dx%d: ",g->x_size, g->y_size);
    for (int i=0; i<NUM_SHAPES; i++)
        printf("%d ", g->shape_count[i]);
    printf("\n");
    for (int y=0; y<g->y_size; y++)
    {
        for (int x=0; x<g->x_size; x++)
            printf("%c", g->current[x][y]?'#':'.');
        printf("\n");
    }
    printf("\n");
}

bool fit_shapes_in_grid(grid *g, shape* shapes)
{
    // Find first shape to fit
    int index=-1;
    for (int i=0; i<NUM_SHAPES; i++)
    {
        if (g->shape_count[i]>0)
        {
            index=i;
            g->shape_count[i]--;
            break;
        }
    }
    if (index<0)
        return true;    // No more shapes, done!
    // Try this shape everywhere, in all orientations
    for (int o=0; o<4; o++)
    {
        for (int x=0; x<g->x_size-2; x++)
        {
            for (int y=0; y<g->y_size-2; y++)
            {
                // printf("Trying shape %d orientation %d at (%d, %d)\n", index, o, x, y);
                // grid new_g=copy_grid(g);
                // print_grid(&new_g);
                if (fit_shape_in_grid(g, &shapes[index], o, x, y))
                {
                    // printf("Shape fits\n");
                    bool success=fit_shapes_in_grid(g, shapes);
                    // print_grid(&new_g);
                    // free_grid(&new_g);
                    if (success)
                        return success;
                }
                // else
                //     free_grid(&new_g);

            }
        }
    }
    return false;
}

void part1()
{
    char **lines;
    size_t num_lines = read_file_of_string_list("days/day12/input.txt", &lines);
    printf("%zu lines\n", num_lines);
    // for (int i=0; i<num_lines; i++)
    //     printf("%d : %s\n", i, lines[i]);
    shape shapes[NUM_SHAPES];
    for (int i=0; i<NUM_SHAPES; i++)
    {
        shapes[i]=new_shape(lines+1+i*5);
        // print_shape(&shapes[i]);
    }
    int count = 0;
    for (int i=5*NUM_SHAPES; i<num_lines; i++)
    {
        grid g=new_grid(lines[i]);
        // print_grid(&g);
        int shape_pixels=0;
        for (int i=0; i<NUM_SHAPES; i++)
            shape_pixels+=g.shape_count[i]*shapes[i].pixels;
        // printf("shape pixels: %d\n", shape_pixels);
        // printf("grid pixels: %d\n", g.x_size*g.y_size);
        bool success=shape_pixels<g.x_size*g.y_size && fit_shapes_in_grid(&g, shapes);
        // printf("%s\n",success?"Success!":"Failure");
        if (success)
            ++count;
        free_grid(&g);
    }
    printf("Part1: %d\n", count);
    free_read_file_of_string_list(lines, num_lines);
}

int main() {
    part1();
    return 0;
}
