#ifndef SAFE_CITIZENS_H
#define SAFE_CITIZENS_H

#include "graph.h"
#include "solution.h"

int find_safe_citizen(Graph* g, int* visited, Solution* current_solution);
Solution find_safe_citizens(Graph* g, int num_citizens, int num_supermarkets);
void run_fscs_process(Graph* g, SharedMemory* shared_mem, int num_citizens, int num_supermarkets);

#endif 