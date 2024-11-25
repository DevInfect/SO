#include "input.h"
#include <stdio.h>
#include <stdlib.h>

int load_input(const char *filename, Mapa *mapa) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Erro ao abrir o ficheiro");
        return 1;
    }

    // Lê o número de avenidas e ruas
    fscanf(file, "%d %d", &mapa->M, &mapa->N);

    // Lê o número de supermercados e cidadãos
    fscanf(file, "%d %d", &mapa->num_supermercados, &mapa->num_cidadaos);

    // Aloca memória para supermercados e cidadãos
    mapa->supermercados = malloc(mapa->num_supermercados * sizeof(Coordenada));
    mapa->cidadaos = malloc(mapa->num_cidadaos * sizeof(Coordenada));

    // Aloca a matriz do grid
    mapa->grid = malloc(mapa->M * sizeof(int*));
    for (int i = 0; i < mapa->M; i++) {
        mapa->grid[i] = calloc(mapa->N, sizeof(int));
    }

    // Lê as coordenadas dos supermercados
    for (int i = 0; i < mapa->num_supermercados; i++) {
        fscanf(file, "%d %d", &mapa->supermercados[i].avenida, &mapa->supermercados[i].rua);
        mapa->grid[mapa->supermercados[i].avenida][mapa->supermercados[i].rua] = 2; // Marca supermercado
    }

    // Lê as coordenadas dos cidadãos
    for (int i = 0; i < mapa->num_cidadaos; i++) {
        fscanf(file, "%d %d", &mapa->cidadaos[i].avenida, &mapa->cidadaos[i].rua);
        mapa->grid[mapa->cidadaos[i].avenida][mapa->cidadaos[i].rua] = 1; // Marca cidadão
    }

    fclose(file);
    return 0;
}

void free_mapa(Mapa *mapa) {
    free(mapa->supermercados);
    free(mapa->cidadaos);
    for (int i = 0; i < mapa->M; i++) {
        free(mapa->grid[i]);
    }
    free(mapa->grid);
}
