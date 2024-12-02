#ifndef UTILS_H
#define UTILS_H

#include "graph.h"
#include "solution.h"

void read_input(char* filename, int* M, int* N, Location** citizens, 
                int* num_citizens, Location** supermarkets, int* num_supermarkets);
void print_solution(Solution* s, long start_time, int iterations, long time_to_best);
long get_current_time_ms();
void validate_input(int M, int N, Location* citizens, int num_citizens, 
                   Location* supermarkets, int num_supermarkets);
void print_test_results(const char* test_name, int total_time, int num_processes,
                       Solution* solution, int iterations, int time_to_best);

#endif