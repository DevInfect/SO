#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/graph.h"

Graph* create_graph(int M, int N) {
    Graph* g = (Graph*)malloc(sizeof(Graph));
    g->M = M;
    g->N = N;
    g->num_vertices = M * N + 2;  // +2 for source and sink vertices

    // Initialize adjacency lists
    for (int i = 0; i < g->num_vertices; i++) {
        g->adjacency_list[i] = (int*)malloc(g->num_vertices * sizeof(int));
        g->list_sizes[i] = 0;
    }

    // Build grid connections
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            int current = get_vertex_number(g, j + 1, i + 1);
            
            // Connect to right neighbor
            if (j < M - 1) {
                int right = get_vertex_number(g, j + 2, i + 1);
                g->adjacency_list[current][g->list_sizes[current]++] = right;
                g->adjacency_list[right][g->list_sizes[right]++] = current;
            }
            
            // Connect to bottom neighbor
            if (i < N - 1) {
                int bottom = get_vertex_number(g, j + 1, i + 2);
                g->adjacency_list[current][g->list_sizes[current]++] = bottom;
                g->adjacency_list[bottom][g->list_sizes[bottom]++] = current;
            }
        }
    }

    return g;
}

void destroy_graph(Graph* g) {
    for (int i = 0; i < g->num_vertices; i++) {
        free(g->adjacency_list[i]);
    }
    free(g);
}

void add_citizen(Graph* g, Location loc) {
    int vertex = get_vertex_number(g, loc.avenue, loc.street);
    g->adjacency_list[0][g->list_sizes[0]++] = vertex;
}

void add_supermarket(Graph* g, Location loc) {
    int vertex = get_vertex_number(g, loc.avenue, loc.street);
    g->adjacency_list[vertex][g->list_sizes[vertex]++] = g->num_vertices - 1;
}

int* get_neighbors(Graph* g, int vertex, int* count) {
    *count = g->list_sizes[vertex];
    return g->adjacency_list[vertex];
}

int get_vertex_number(Graph* g, int avenue, int street) {
    return (street - 1) * g->M + avenue;
} 