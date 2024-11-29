#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "../include/utils.h"

void read_input(char* filename, int* M, int* N, Location** citizens, int* num_citizens, 
                Location** supermarkets, int* num_supermarkets) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    // Read grid dimensions
    fscanf(file, "%d %d", M, N);
    
    // Read counts
    fscanf(file, "%d %d", num_supermarkets, num_citizens);

    // Allocate and read supermarkets
    *supermarkets = (Location*)malloc(*num_supermarkets * sizeof(Location));
    for (int i = 0; i < *num_supermarkets; i++) {
        fscanf(file, "%d %d", &(*supermarkets)[i].avenue, &(*supermarkets)[i].street);
    }

    // Allocate and read citizens
    *citizens = (Location*)malloc(*num_citizens * sizeof(Location));
    for (int i = 0; i < *num_citizens; i++) {
        fscanf(file, "%d %d", &(*citizens)[i].avenue, &(*citizens)[i].street);
    }

    fclose(file);
}

void print_solution(Solution* s) {
    printf("Number of safe citizens: %d\n", s->score);
    printf("Paths:\n");
    for (int i = 0; i < s->num_paths; i++) {
        printf("Citizen %d: ", i + 1);
        for (int j = 0; j < s->paths[i].path_length; j++) {
            printf("%d ", s->paths[i].path[j]);
        }
        printf("\n");
    }
}

long get_current_time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
} 