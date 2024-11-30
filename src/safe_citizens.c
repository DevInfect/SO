#include <stdlib.h>
#include <string.h>
#include "../include/safe_citizens.h"

#define MAX_VISITED 1000

int find_safe_citizen(Graph* g, int* visited, Solution* current_solution) {
    int* current_path = (int*)malloc(MAX_PATH_LENGTH * sizeof(int));
    int path_length = 0;
    int* next_nodes = (int*)malloc(g->num_vertices * sizeof(int));
    int next_nodes_count = 0;
    
    // Get initial nodes (connected to source)
    int* neighbors = get_neighbors(g, 0, &next_nodes_count);
    for (int i = 0; i < next_nodes_count; i++) {
        if (!visited[neighbors[i]]) {
            // Start new path from source to this citizen
            path_length = 0;
            current_path[path_length++] = 0;  // source
            current_path[path_length++] = neighbors[i];  // citizen
            visited[neighbors[i]] = 1;
            
            // Try to find path to a supermarket
            int current = neighbors[i];
            int found_path = 0;
            
            while (!found_path) {
                int neighbor_count;
                int* current_neighbors = get_neighbors(g, current, &neighbor_count);
                
                // Check if connected to sink (supermarket found)
                for (int j = 0; j < neighbor_count; j++) {
                    if (current_neighbors[j] == g->num_vertices - 1) {
                        current_path[path_length++] = g->num_vertices - 1;
                        add_path_to_solution(current_solution, current_path, path_length);
                        free(current_path);
                        free(next_nodes);
                        return 1;
                    }
                }
                
                // Try next unvisited neighbor
                found_path = 0;
                for (int j = 0; j < neighbor_count; j++) {
                    int next = current_neighbors[j];
                    if (!visited[next] && next != g->num_vertices - 1) {
                        current_path[path_length++] = next;
                        visited[next] = 1;
                        current = next;
                        found_path = 1;
                        break;
                    }
                }
                
                if (!found_path) break;  // No more paths to try
            }
        }
    }
    
    free(current_path);
    free(next_nodes);
    return 0;
}

Solution find_safe_citizens(Graph* g, int num_citizens, int num_supermarkets) {
    Solution solution = create_solution();
    int* visited = (int*)calloc(g->num_vertices, sizeof(int));
    int max_solutions = num_citizens < num_supermarkets ? num_citizens : num_supermarkets;

    while (solution.score < max_solutions) {
        int result = find_safe_citizen(g, visited, &solution);
        if (!result) break;
    }

    free(visited);
    return solution;
}

void run_fscs_process(Graph* g, SharedMemory* shared_mem, int num_citizens, int num_supermarkets) {
    long start_time = get_current_time_ms();
    
    while (1) {
        Solution current_solution = find_safe_citizens(g, num_citizens, num_supermarkets);
        
        sem_wait(shared_mem->mutex);
        shared_mem->iterations++;
        
        if (current_solution.score > shared_mem->best_solution.score) {
            destroy_solution(&shared_mem->best_solution);
            copy_solution(&shared_mem->best_solution, &current_solution);
            shared_mem->time_to_best = get_current_time_ms() - start_time;
        }
        
        sem_post(shared_mem->mutex);
        destroy_solution(&current_solution);
    }
} 