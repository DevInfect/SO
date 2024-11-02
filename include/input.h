#ifndef INPUT_H
#define INPUT_H

typedef struct {
    int avenue;
    int street;
} Coordinate;

typedef struct {
    int M;
    int N;
    Coordinate *supermarket;
    Coordinate *people;
} Map;

void load_input(const char *filename, Map *map);

#endif