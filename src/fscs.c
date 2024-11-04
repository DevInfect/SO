#include "fscs.h"
#include <stdlib.h>
#include <stdio.h>

static int caminho_valido(Coordenada pos, Caminho *caminhos, int num_cidadaos) {
    for (int i = 0; i < num_cidadaos; i++) {
        for (int j = 0; j < caminhos[i].tamanho; j++) {
            if (caminhos[i].caminho[j].avenida == pos.avenida &&
                caminhos[i].caminho[j].rua == pos.rua) {
                return 0; // Cruzamento já visitado
            }
        }
    }
    return 1; // Cruzamento livre
}

int find_safe_citizen(const Mapa *mapa, Caminho *caminhos) {
    int num_solucoes = 0;

    // Percorre todos os cidadãos
    for (int cidadao = 0; cidadao < mapa->num_cidadaos; cidadao++) {
        Coordenada pos_atual = mapa->cidadaos[cidadao];
        caminhos[cidadao].tamanho = 0;
        caminhos[cidadao].caminho = malloc((mapa->M + mapa->N) * sizeof(Coordenada));

        int encontrou_supermercado = 0;

        // Busca um supermercado
        for (int s = 0; s < mapa->num_supermercados; s++) {
            Coordenada destino = mapa->supermercados[s];

            // Caminho direto em Manhattan: primeiro move-se na avenida, depois na rua
            Coordenada passo;

            // Movendo na direção horizontal (avenida)
            while (pos_atual.avenida != destino.avenida) {
                passo = pos_atual;
                passo.avenida += (destino.avenida > pos_atual.avenida) ? 1 : -1;
                if (!caminho_valido(passo, caminhos, cidadao)) break;

                caminhos[cidadao].caminho[caminhos[cidadao].tamanho++] = passo;
                pos_atual = passo;
            }

            // Movendo na direção vertical (rua)
            while (pos_atual.rua != destino.rua && caminho_valido(pos_atual, caminhos, cidadao)) {
                passo = pos_atual;
                passo.rua += (destino.rua > pos_atual.rua) ? 1 : -1;
                if (!caminho_valido(passo, caminhos, cidadao)) break;

                caminhos[cidadao].caminho[caminhos[cidadao].tamanho++] = passo;
                pos_atual = passo;
            }

            // Verifica se chegou ao supermercado
            if (pos_atual.avenida == destino.avenida && pos_atual.rua == destino.rua) {
                encontrou_supermercado = 1;
                num_solucoes++;
                break;
            }
        }

        // Se não encontrou um caminho, libera a memória do caminho desse cidadão
        if (!encontrou_supermercado) {
            free(caminhos[cidadao].caminho);
            caminhos[cidadao].caminho = NULL;
        }
    }
    return num_solucoes;
}
