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

    // Lê as coordenadas dos supermercados
    for (int i = 0; i < mapa->num_supermercados; i++) {
        fscanf(file, "%d %d", &mapa->supermercados[i].avenida, &mapa->supermercados[i].rua);
    }

    // Lê as coordenadas dos cidadãos
    for (int i = 0; i < mapa->num_cidadaos; i++) {
        fscanf(file, "%d %d", &mapa->cidadaos[i].avenida, &mapa->cidadaos[i].rua);
    }

    fclose(file);
    return 0;
}
