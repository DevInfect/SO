#include <stdlib.h>
#include <string.h>
#include "../include/solution.h"

Solution create_solution() {
    Solution s;
    s.num_paths = 0;
    s.score = 0;
    return s;
}

void destroy_solution(Solution* s) {
    for (int i = 0; i < s->num_paths; i++) {
        free(s->paths[i].path);
    }
    s->num_paths = 0;
    s->score = 0;
}

void copy_solution(Solution* dest, Solution* src) {
    dest->num_paths = src->num_paths;
    dest->score = src->score;
    
    for (int i = 0; i < src->num_paths; i++) {
        dest->paths[i].path_length = src->paths[i].path_length;
        dest->paths[i].path = (int*)malloc(src->paths[i].path_length * sizeof(int));
        memcpy(dest->paths[i].path, src->paths[i].path, 
               src->paths[i].path_length * sizeof(int));
    }
}

void add_path_to_solution(Solution* s, int* path, int path_length) {
    if (s->num_paths < MAX_PATHS) {
        s->paths[s->num_paths].path = (int*)malloc(path_length * sizeof(int));
        memcpy(s->paths[s->num_paths].path, path, path_length * sizeof(int));
        s->paths[s->num_paths].path_length = path_length;
        s->num_paths++;
        s->score++;
    }
} 