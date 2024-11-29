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
    if (fscanf(file, "%d %d", M, N) != 2) {
        fprintf(stderr, "Error reading grid dimensions\n");
        fclose(file);
        exit(1);
    }
    
    // Read counts
    if (fscanf(file, "%d %d", num_supermarkets, num_citizens) != 2) {
        fprintf(stderr, "Error reading counts\n");
        fclose(file);
        exit(1);
    }

    // Validate basic input
    if (*M <= 0 || *N <= 0 || *M * *N + 2 > MAX_VERTICES) {
        fprintf(stderr, "Invalid grid dimensions: M=%d, N=%d\n", *M, *N);
        fclose(file);
        exit(1);
    }

    if (*num_supermarkets <= 0 || *num_citizens <= 0) {
        fprintf(stderr, "Invalid counts: supermarkets=%d, citizens=%d\n", 
                *num_supermarkets, *num_citizens);
        fclose(file);
        exit(1);
    }

    // Allocate memory
    *supermarkets = (Location*)malloc(*num_supermarkets * sizeof(Location));
    *citizens = (Location*)malloc(*num_citizens * sizeof(Location));
    
    if (!*supermarkets || !*citizens) {
        fprintf(stderr, "Memory allocation failed\n");
        fclose(file);
        exit(1);
    }

    // Read supermarket locations
    for (int i = 0; i < *num_supermarkets; i++) {
        if (fscanf(file, "%d %d", &(*supermarkets)[i].avenue, &(*supermarkets)[i].street) != 2) {
            fprintf(stderr, "Error reading supermarket coordinates at index %d\n", i);
            free(*supermarkets);
            free(*citizens);
            fclose(file);
            exit(1);
        }
        
        // Validate coordinates
        if ((*supermarkets)[i].avenue < 1 || (*supermarkets)[i].avenue > *M ||
            (*supermarkets)[i].street < 1 || (*supermarkets)[i].street > *N) {
            fprintf(stderr, "Invalid supermarket coordinates at index %d: (%d,%d)\n",
                    i, (*supermarkets)[i].avenue, (*supermarkets)[i].street);
            free(*supermarkets);
            free(*citizens);
            fclose(file);
            exit(1);
        }
    }

    // Read citizen locations
    for (int i = 0; i < *num_citizens; i++) {
        if (fscanf(file, "%d %d", &(*citizens)[i].avenue, &(*citizens)[i].street) != 2) {
            fprintf(stderr, "Error reading citizen coordinates at index %d\n", i);
            free(*supermarkets);
            free(*citizens);
            fclose(file);
            exit(1);
        }
        
        // Validate coordinates
        if ((*citizens)[i].avenue < 1 || (*citizens)[i].avenue > *M ||
            (*citizens)[i].street < 1 || (*citizens)[i].street > *N) {
            fprintf(stderr, "Invalid citizen coordinates at index %d: (%d,%d)\n",
                    i, (*citizens)[i].avenue, (*citizens)[i].street);
            free(*supermarkets);
            free(*citizens);
            fclose(file);
            exit(1);
        }
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

void validate_input(int M, int N, Location* citizens, int num_citizens, 
                   Location* supermarkets, int num_supermarkets) {
    if (M <= 0 || N <= 0 || M * N + 2 > MAX_VERTICES) {
        fprintf(stderr, "Invalid grid dimensions\n");
        exit(1);
    }

    if (num_citizens <= 0 || num_supermarkets <= 0) {
        fprintf(stderr, "Invalid number of citizens or supermarkets\n");
        exit(1);
    }

    // Check coordinates
    for (int i = 0; i < num_supermarkets; i++) {
        if (supermarkets[i].avenue < 1 || supermarkets[i].avenue > M ||
            supermarkets[i].street < 1 || supermarkets[i].street > N) {
            fprintf(stderr, "Invalid supermarket coordinates\n");
            exit(1);
        }
        // Check duplicates
        for (int j = 0; j < i; j++) {
            if (supermarkets[i].avenue == supermarkets[j].avenue &&
                supermarkets[i].street == supermarkets[j].street) {
                fprintf(stderr, "Duplicate supermarket locations\n");
                exit(1);
            }
        }
    }

    // Similar checks for citizens
    for (int i = 0; i < num_citizens; i++) {
        if (citizens[i].avenue < 1 || citizens[i].avenue > M ||
            citizens[i].street < 1 || citizens[i].street > N) {
            fprintf(stderr, "Invalid citizen coordinates\n");
            exit(1);
        }
        // Check duplicates
        for (int j = 0; j < i; j++) {
            if (citizens[i].avenue == citizens[j].avenue &&
                citizens[i].street == citizens[j].street) {
                fprintf(stderr, "Duplicate citizen locations\n");
                exit(1);
            }
        }
    }
}

void print_test_results(const char* test_name, int total_time, int num_processes,
                       Solution* solution, int iterations, int time_to_best) {
    printf("Test Results for %s:\n", test_name);
    printf("Total execution time: %d ms\n", total_time);
    printf("Number of processes: %d\n", num_processes);
    printf("Safe citizens: %d\n", solution->score);
    printf("Paths:\n");
    for (int i = 0; i < solution->num_paths; i++) {
        printf("  Citizen %d:", i + 1);
        for (int j = 0; j < solution->paths[i].path_length; j++) {
            printf(" %d", solution->paths[i].path[j]);
        }
        printf("\n");
    }
    printf("Total iterations: %d\n", iterations);
    printf("Time to best solution: %d ms\n", time_to_best);
    printf("----------------------------------------\n");
}