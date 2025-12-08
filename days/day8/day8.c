#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "file.h"

typedef struct node_s {
    double x;
    double y;
    double z;
    int circuit;
} node;

double calc_distance(node a, node b)
{
    double d2 = pow(a.x - b.x, 2);
    d2 += pow(a.y - b.y, 2);
    d2 += pow(a.z - b.z, 2);
    return sqrt(d2);
}

typedef struct pair_s {
    node* a;
    node* b;
    double distance;
} pair;

int compare_pair(const void* va, const void* vb)
{
    const pair *a = (const pair*)va;
    const pair *b = (const pair*)vb;
    if (a->distance > b->distance) return 1;
    if (a->distance < b->distance) return -1;
    return 0;
}

int compare_ints_desc(const void* va, const void* vb)
{
    int a = *(const int*)va;
    int b = *(const int*)vb;
    if (a>b) return -1;
    if (a<b) return 1;
    return 0;
}

void part1()
{
    int_array *int_rows;
    const int NUM_PAIRS_TO_JOIN = 1000;
    size_t num_rows = read_file_of_int_rows("days/day8/input.txt", &int_rows);
    printf("%zu rows\n", num_rows);
    node* nodes = (node*)malloc(num_rows * sizeof(node));
    for (int i=0; i<num_rows; i++)
    {
        nodes[i].x = int_rows[i].elements[0];
        nodes[i].y = int_rows[i].elements[1];
        nodes[i].z = int_rows[i].elements[2];
        nodes[i].circuit = -1;
    }
    free_file_of_int_rows(num_rows, int_rows);
    // Number of possible distinct pairs, 99 + 98 + 97 + ..., if num_rows is 100
    int num_pairs = num_rows*(num_rows-1)/2;
    pair* pairs = (pair*)malloc(num_pairs * sizeof(pair));
    int p=0;
    for (int i=0; i<num_rows; i++)
        for (int j=i+1; j<num_rows; j++, p++)
        {
            pairs[p].a = &nodes[i];
            pairs[p].b = &nodes[j];
            pairs[p].distance = calc_distance(nodes[i], nodes[j]);
        }
    qsort(pairs, num_pairs, sizeof(pair), compare_pair);
    int next_circuit=0;
    int *circuit_sizes = (int*)malloc(num_rows * sizeof(int));
    for (int i=0; i<num_rows; circuit_sizes[i++]=0)
        ;
    for (int i=0; i<NUM_PAIRS_TO_JOIN; i++)
    {
        node *a = pairs[i].a;
        node *b = pairs[i].b;
        if (a->circuit == -1 && b->circuit == -1)
        {
            // New circuit
            a->circuit = next_circuit;
            b->circuit = next_circuit;
            circuit_sizes[next_circuit] = 2;
            next_circuit++;
        }
        else if (a->circuit != -1 && b->circuit == -1)
        {
            // Node A in existing circuit
            b->circuit = a->circuit;
            circuit_sizes[a->circuit]++;
        }
        else if (a->circuit == -1 && b->circuit != -1)
        {
            // Node B in existing circuit
            a->circuit = b->circuit;
            circuit_sizes[b->circuit]++;
        }
        else
        {
            if (a->circuit == b->circuit)
            {
                continue;
            }
            // Both in existing circuits
            // Combine the sizes into one
            circuit_sizes[a->circuit]+=circuit_sizes[b->circuit];
            circuit_sizes[b->circuit]=0;
            int b_circuit = b->circuit;
            // Move all nodes in the B circuit into the A circuit
            for (int j=0; j<num_rows; j++)
            {
                if (nodes[j].circuit == b_circuit)
                    nodes[j].circuit = a->circuit;
            }
        }
    }
    qsort(circuit_sizes, num_rows, sizeof(int), compare_ints_desc);
    int64_t product = circuit_sizes[0] * circuit_sizes[1] * circuit_sizes[2];
    printf("Part1: %ld\n", product);
}

void part2()
{
    int_array *int_rows;
    size_t num_rows = read_file_of_int_rows("days/day8/input.txt", &int_rows);
    printf("%zu rows\n", num_rows);
    node* nodes = (node*)malloc(num_rows * sizeof(node));
    for (int i=0; i<num_rows; i++)
    {
        nodes[i].x = int_rows[i].elements[0];
        nodes[i].y = int_rows[i].elements[1];
        nodes[i].z = int_rows[i].elements[2];
        nodes[i].circuit = -1;
    }
    free_file_of_int_rows(num_rows, int_rows);
    // Number of possible distinct pairs, 99 + 98 + 97 + ..., if num_rows is 100
    int num_pairs = num_rows*(num_rows-1)/2;
    pair* pairs = (pair*)malloc(num_pairs * sizeof(pair));
    int p=0;
    for (int i=0; i<num_rows; i++)
        for (int j=i+1; j<num_rows; j++, p++)
        {
            pairs[p].a = &nodes[i];
            pairs[p].b = &nodes[j];
            pairs[p].distance = calc_distance(nodes[i], nodes[j]);
        }
    qsort(pairs, num_pairs, sizeof(pair), compare_pair);
    int next_circuit=0;
    int *circuit_sizes = (int*)malloc(num_rows * sizeof(int));
    for (int i=0; i<num_rows; circuit_sizes[i++]=0)
        ;
    for (int i=0; ; i++)
    {
        node *a = pairs[i].a;
        node *b = pairs[i].b;
        if (a->circuit == -1 && b->circuit == -1)
        {
            // New circuit
            a->circuit = next_circuit;
            b->circuit = next_circuit;
            circuit_sizes[next_circuit] = 2;
            next_circuit++;
        }
        else if (a->circuit != -1 && b->circuit == -1)
        {
            // Node A in existing circuit
            b->circuit = a->circuit;
            circuit_sizes[a->circuit]++;
        }
        else if (a->circuit == -1 && b->circuit != -1)
        {
            // Node B in existing circuit
            a->circuit = b->circuit;
            circuit_sizes[b->circuit]++;
        }
        else
        {
            if (a->circuit == b->circuit)
            {
                continue;
            }
            // Both in existing circuits
            // Combine the sizes into one
            circuit_sizes[a->circuit]+=circuit_sizes[b->circuit];
            circuit_sizes[b->circuit]=0;
            int b_circuit = b->circuit;
            // Move all nodes in the B circuit into the A circuit
            for (int j=0; j<num_rows; j++)
            {
                if (nodes[j].circuit == b_circuit)
                    nodes[j].circuit = a->circuit;
            }
        }
        if (circuit_sizes[a->circuit] == num_rows || circuit_sizes[b->circuit] == num_rows)
        {
            int64_t product = (int64_t)(a->x) * (int64_t)(b->x);
            printf("Part2: %ld\n", product);
            break;
        }
    }
}

int main() {
    part1();
    part2();
    return 0;
}
