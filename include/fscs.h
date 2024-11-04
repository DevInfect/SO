#ifndef FSCS_H
#define FSCS_H

#include "input.h"

typedef struct {
    Coordenada *caminho;
    int tamanho;
} Caminho;

int find_safe_citizen(const Mapa *mapa, Caminho *caminhos);

#endif // FSCS_H
