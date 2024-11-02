#ifndef FSCS_H
#define FSCH_H

#include "input.h"


 //typedef struct for list of paths
typedef struct {
    Coordinate *path;
    int length;
} Path;

//function find safe citizen 
int find_safe_citizen( const Map *map, Path *paths, int num_people);

#endif