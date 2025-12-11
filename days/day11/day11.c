#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file.h"

typedef struct node_s {
    char name[4];
    int num_children;
    char** children;
    int cached_paths_to_out;
} node;

node new_node(char* line)
{
    node n;
    n.num_children = strlen(line)/4-1;
    n.children = (char**)malloc(n.num_children*sizeof(char*));
    for (char* c=line; *c; c++)
        if (*c==' ' || *c==':')
            *c = '\0';
    n.cached_paths_to_out = -1;
    strcpy(n.name, line);
    for (int i=0; i<n.num_children; i++)
    {
        n.children[i] = (char*)malloc(4);
        strcpy(n.children[i], line+5+i*4);
    }
    return n;
}

void free_node(node* n)
{
    for (int i=0; i<n->num_children; i++)
        free(n->children[i]);
    free(n->children);
}

void print_node(const node* n)
{
    printf("node.name %s\n", n->name);
    printf("node.cached_paths_to_out %d\n", n->cached_paths_to_out);
    printf("node.num_children %d\n", n->num_children);
    for (int i=0; i<n->num_children; i++)
        printf("node.children[%d] = \"%s\"\n", i, n->children[i]);
}

int compare_nodes(const void* a, const void* b)
{
    return strcmp(
        ((node*)a)->name,
        ((node*)b)->name
    );
}

int find_num_paths_to_out(char *node_name, node** nodes, size_t num_nodes)
{
    if (strcmp(node_name, "out")==0)
        return 1;
    node key;
    strcpy(key.name, node_name);
    node* n = bsearch(&key, *nodes, num_nodes, sizeof(node), compare_nodes);
    if (n->cached_paths_to_out > 0)
    {
        // printf("Used cached value for %s, %d\n", node_name, n->cached_paths_to_out);
        return n->cached_paths_to_out;
    }
    int num_paths = 0;
    for (int i=0; i<n->num_children; i++)
        num_paths += find_num_paths_to_out(n->children[i], nodes, num_nodes);
    n->cached_paths_to_out = num_paths;
    return num_paths;
}

void part1()
{
    char **lines;
    size_t num_lines = read_file_of_string_list("days/day11/input.txt", &lines);
    printf("%zu lines\n", num_lines);
    node *nodes;
    nodes = (node*)malloc(num_lines*sizeof(node));
    for (int i=0; i<num_lines; i++)
    {
        // printf("%d : %s\n", i, lines[i]);
        nodes[i] = new_node(lines[i]);
    }
    free_read_file_of_string_list(lines, num_lines);
    qsort(nodes, num_lines, sizeof(node), compare_nodes);
    // for (int i=0; i<num_lines; i++)
    // {
    //     print_node(&nodes[i]);
    // }
    printf("Part1: %d\n", find_num_paths_to_out("you", &nodes, num_lines));
    // Still need to free some stuff!
    for (int i=0; i<num_lines; i++)
        free_node(&nodes[i]);
    free(nodes);
}

int main() {
    part1();
    return 0;
}
