#include <input.h>
#include <stdio.h>
#include <stdlib.h>
// function load_input that loads the file and saves it in the map
void load_input(const char *filename, Map *map) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: could not open file %s\n", filename);
        exit(1);
    }

    // read the number of avenues and streets
    fscanf(file, "%d %d", &map->M, &map->N);

    // read the number of supermarkets and people
    int num_supermarkets, num_people;
    fscanf(file, "%d %d", &num_supermarkets, &num_people);

    // allocate memory for the arrays of coordinates
    map->supermarket = malloc(num_supermarkets * sizeof(Coordinate));
    map->people = malloc(num_people * sizeof(Coordinate));

    // read the coordinates of the supermarkets
    for (int i = 0; i < num_supermarkets; i++) {
        fscanf(file, "%d %d", &map->supermarket[i].avenue, &map->supermarket[i].street);
    }

    // read the coordinates of the people
    for (int i = 0; i < num_people; i++) {
        fscanf(file, "%d %d", &map->people[i].avenue, &map->people[i].street);
    }

    fclose(file);
}