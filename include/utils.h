#ifndef UTILS_H
#define UTILS_H

#include "graph.h"

void read_input(char* filename, int* M, int* N, Location** citizens, int* num_citizens, 
                Location** supermarkets, int* num_supermarkets);
void print_solution(Solution* s);
long get_current_time_ms();

#endif 