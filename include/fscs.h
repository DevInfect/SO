#ifndef FSCS_H
#define FSCS_H

#include "input.h"

typedef struct {
    Coordenada *caminho;
    int tamanho;
    int cidadao_id;  // ID do cidadão que usa este caminho
} Caminho;

// Encontra caminhos seguros para os cidadãos
int find_safe_paths(const Mapa *mapa, Caminho *caminhos);

// Libera a memória dos caminhos
void free_caminhos(Caminho *caminhos, int num_cidadaos);

#endif // FSCS_H
