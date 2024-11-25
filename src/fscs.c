#include "fscs.h"
#include <stdlib.h>
#include <string.h>

static int is_valid_move(const Mapa *mapa, Coordenada pos, Coordenada *visited, int visit_count) {
    // Verifica se está dentro dos limites
    if (pos.avenida < 0 || pos.avenida >= mapa->M || 
        pos.rua < 0 || pos.rua >= mapa->N)
        return 0;

    // Verifica se o ponto já foi visitado
    for (int i = 0; i < visit_count; i++) {
        if (visited[i].avenida == pos.avenida && visited[i].rua == pos.rua)
            return 0;
    }

    return 1;
}

static int find_path_to_supermarket(const Mapa *mapa, Coordenada start, 
                                  Caminho *caminho, int cidadao_id) {
    int max_path = mapa->M * mapa->N;
    Coordenada *visited = malloc(max_path * sizeof(Coordenada));
    int visit_count = 0;

    // Inicializa o caminho
    caminho->cidadao_id = cidadao_id;
    caminho->tamanho = 0;
    caminho->caminho = malloc(max_path * sizeof(Coordenada));
    
    // Adiciona posição inicial
    visited[visit_count++] = start;
    caminho->caminho[caminho->tamanho++] = start;

    // Direções possíveis (norte, sul, leste, oeste)
    const int dx[] = {0, 0, 1, -1};
    const int dy[] = {1, -1, 0, 0};

    // Para cada supermercado, tenta encontrar um caminho
    for (int s = 0; s < mapa->num_supermercados; s++) {
        Coordenada current = start;
        Coordenada target = mapa->supermercados[s];
        
        // Tenta chegar ao supermercado movendo-se primeiro na horizontal
        while (current.avenida != target.avenida) {
            Coordenada next = current;
            next.avenida += (target.avenida > current.avenida) ? 1 : -1;
            
            if (!is_valid_move(mapa, next, visited, visit_count))
                continue;
                
            current = next;
            visited[visit_count++] = current;
            caminho->caminho[caminho->tamanho++] = current;
        }
        
        // Depois move-se na vertical
        while (current.rua != target.rua) {
            Coordenada next = current;
            next.rua += (target.rua > current.rua) ? 1 : -1;
            
            if (!is_valid_move(mapa, next, visited, visit_count))
                continue;
                
            current = next;
            visited[visit_count++] = current;
            caminho->caminho[caminho->tamanho++] = current;
        }
        
        // Se chegou ao destino
        if (current.avenida == target.avenida && current.rua == target.rua) {
            free(visited);
            return 1;
        }
    }

    // Não encontrou caminho
    free(visited);
    free(caminho->caminho);
    caminho->caminho = NULL;
    caminho->tamanho = 0;
    return 0;
}

int find_safe_paths(const Mapa *mapa, Caminho *caminhos) {
    int num_safe = 0;
    
    for (int i = 0; i < mapa->num_cidadaos; i++) {
        if (find_path_to_supermarket(mapa, mapa->cidadaos[i], &caminhos[i], i)) {
            num_safe++;
        }
    }
    
    return num_safe;
}

void free_caminhos(Caminho *caminhos, int num_cidadaos) {
    for (int i = 0; i < num_cidadaos; i++) {
        if (caminhos[i].caminho != NULL) {
            free(caminhos[i].caminho);
        }
    }
}
