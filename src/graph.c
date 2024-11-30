#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/graph.h"

Graph* create_graph(int M, int N) {
    Graph* g = (Graph*)malloc(sizeof(Graph));
    if (!g) {
        fprintf(stderr, "Failed to allocate graph\n");
        exit(1);
    }
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
    g->adjacency_list[vertex][g->list_sizes[vertex]++] = 0;  // Add bidirectional edge
}

void add_supermarket(Graph* g, Location loc) {
    int vertex = get_vertex_number(g, loc.avenue, loc.street);
    int sink = g->num_vertices - 1;
    g->adjacency_list[vertex][g->list_sizes[vertex]++] = sink;
    g->adjacency_list[sink][g->list_sizes[sink]++] = vertex;  // Add bidirectional edge
}

int* get_neighbors(Graph* g, int vertex, int* count) {
    *count = g->list_sizes[vertex];
    return g->adjacency_list[vertex];
}

int get_vertex_number(Graph* g, int avenue, int street) {
    if (avenue < 1 || avenue > g->M || street < 1 || street > g->N) {
        fprintf(stderr, "Invalid coordinates: avenue=%d, street=%d\n", avenue, street);
        exit(1);
    }
    return 1 + (street - 1) * g->M + (avenue - 1);
} 