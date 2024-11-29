#ifndef GRAPH_H
#define GRAPH_H

#include <stdlib.h>

#define MAX_VERTICES 1000

typedef struct {
    int avenue;
    int street;
} Location;

typedef struct {
    int num_vertices;
    int *adjacency_list[MAX_VERTICES];
    int list_sizes[MAX_VERTICES];
    int M;  // number of avenues
    int N;  // number of streets
} Graph;

Graph* create_graph(int M, int N);
void destroy_graph(Graph* g);
void add_citizen(Graph* g, Location loc);
void add_supermarket(Graph* g, Location loc);
int* get_neighbors(Graph* g, int vertex, int* count);
int get_vertex_number(Graph* g, int avenue, int street);

#endif 