#ifndef SOLUTION_H
#define SOLUTION_H

#include <semaphore.h>

#define MAX_PATHS 100
#define MAX_PATH_LENGTH 1000

typedef struct {
    int* path;
    int path_length;
} Path;

typedef struct {
    int num_paths;
    Path paths[MAX_PATHS];
    int score;
} Solution;

typedef struct {
    Solution best_solution;
    sem_t* mutex;
    int iterations;
    long time_to_best;
    int optimal_solutions_found;
    double avg_execution_time;
    double avg_iterations;
} SharedMemory;

Solution create_solution();
void destroy_solution(Solution* s);
void copy_solution(Solution* dest, Solution* src);
void add_path_to_solution(Solution* s, int* path, int path_length);

#endif 